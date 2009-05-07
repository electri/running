/****************************************************************************

	running - A small program to keep track of your workouts.
	Copyright (C) 2009  Marco Gasparetto (markgabbahey@gmail.com)

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

****************************************************************************/

#include <QtGui>
#include <QtSql>
#include "database.h"

const QString DATABASENAME = "running.db";
const int DATABASEVERSION = 2;

QString Database::sm_lastError = "";

/*
 * Open and initialize the database. First is checked if the file exists. If
 * true is checked the existing database version, and if needed is performed an
 * upgrade. If the file didn't exist a new database is created and initialized.
 */
bool Database::init()
{
	sm_lastError = "";

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(DATABASENAME);

	QDir::setCurrent(QApplication::applicationDirPath());

	if (QFile::exists(DATABASENAME)) {
		if (!db.open()) {
			sm_lastError = tr("Unable to open existing database file.");
			return false;
		}
		int dbver = _databaseVersion(db);
		if (dbver > DATABASEVERSION)
		{
			sm_lastError = tr("The database file is for a newer version of the software.");
			return false;
		} else if (dbver < DATABASEVERSION) {
			if (!_upgradeDatabase(db, dbver)) {
				sm_lastError = tr("An error has occurred upgrading the database.");
				return false;
			}
		}
	} else {
		if (!db.open()) {
			sm_lastError = tr("Unable to create new database file.");
			return false;
		}
		if (!_createDatabase(db)) {
			sm_lastError = tr("An error has occurred creating a new database.");
			return false;
		}
	}

	return true;
}

/*
 * Close and remove the database.
 */
void Database::close()
{
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isValid()) {
		if (db.isOpen()) {
			db.close();
		}
		db.removeDatabase(DATABASENAME);
	}
}

/*
 * Return the last error.
 */
QString Database::lastError()
{
	QString message;

	if (!sm_lastError.isEmpty()) {
		message += QString("%1").arg(sm_lastError);
	} else {
		message += "Unknown error";
	}

	message = tr("A database error has occurred:\n\n") + message;

	return message;
}

/*
 * Check the database version (the user version).
 */
int Database::_databaseVersion(QSqlDatabase &db)
{
	QSqlQuery query(db);
	query.exec("PRAGMA user_version");
	if (query.next()) {
		return query.value(0).toInt();
	}
	return 0;
}

/*
 * Perform a new database creation. It call _alterDatabase with the "create"
 * script name.
 */
bool Database::_createDatabase(QSqlDatabase &db)
{
	bool rc = _alterDatabase(db, "create", tr("Creating database ..."));
	if (!rc) {
		db.close();
		QFile::remove(DATABASENAME);
		return false;
	}
	return true;
}

/*
 * Perform a database upgrade from oldVersion to "oldVersion + 1". It call
 * _alterDatabase with the generated upgrade script name. A backup copy of the
 * original file is made.
 */
bool Database::_upgradeDatabase(QSqlDatabase &db, int oldVersion)
{
	bool result = true;

	if (!QFile::copy(DATABASENAME, DATABASENAME + ".bkp")) {
		return false;
	}

	for (int i = oldVersion; i < DATABASEVERSION; ++i) {
		QString scriptName = QString("upgrade_%1-%2").arg(i).arg(i + 1);
		result &= _alterDatabase(db, scriptName, tr("Upgrading database ..."));
	}

	return result;
}

/*
 * Execute a resource stored SQL script. Can be a text file with queries for
 * perform a database new creation, or a upgrade. The text file must be a list
 * of queries separated by blank lines. While executing queries it display a
 * progress bar. If exists an initialization script for the upgrade it is
 * also executed (<scriptName>_init).
 *
 * TODO: Enclose the queries in a transaction.
 */
bool Database::_alterDatabase(QSqlDatabase &db, const QString &scriptName, const QString &message)
{
	qDebug() << "[ALTER DATABASE] Executing: " << scriptName;

	QProgressDialog progress;
	progress.setWindowModality(Qt::WindowModal);
	progress.setLabelText(message);

	QStringList queries;

	QFile file(QString(":/sql/%1.sql").arg(scriptName));
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return false;
	}
	QTextStream textStream(&file);
	QString buffer = textStream.readAll();
	file.close();
	queries << buffer.split("\n\n", QString::SkipEmptyParts);

	QFile file1(QString(":/sql/%1_init.sql").arg(scriptName));
	if (file1.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream textStream1(&file1);
		buffer = textStream1.readAll();
		file1.close();
		queries << buffer.split("\n\n", QString::SkipEmptyParts);
	}

	progress.setMinimum(0);
	progress.setMaximum(queries.count());

	QSqlQuery query(db);
	int i = 0;
	foreach (QString text, queries) {
		progress.setValue(++i);
		qApp->processEvents();

		if (!query.exec(text)) {
			qDebug() << "[ALTER DATABASE] Error executing: " << text;
			return false;
		}
	}

	return true;
}
