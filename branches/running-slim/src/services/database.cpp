/****************************************************************************

	running - A small program to keep track of your workouts.
	Copyright (C) 2008  Marco Gasparetto (markgabbahey@gmail.com)

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

const QString DATABASENAME = "running.db3";
const int DATABASEVERSION = 1;

namespace Services {

Database::Database()
{
	m_active = false;

	m_database = QSqlDatabase::addDatabase("QSQLITE");
	m_database.setDatabaseName(DATABASENAME);

	QDir::setCurrent(QApplication::applicationDirPath());

	if (QFile::exists(DATABASENAME)) {
		if (!m_database.open()) {
			m_lastError = "Unable to open existing database file.";
			return;
		}
		int databaseVersion = this->databaseVersion();
		if (databaseVersion > DATABASEVERSION)
		{
			m_lastError = "The database file is for a newer version of the software.";
			return;
		} else if (databaseVersion < DATABASEVERSION) {
			if (!this->upgradeDatabase(databaseVersion)) {
				m_lastError = "An error has occurred upgrading the database.";
				return;
			}
		}
	} else {
		if (!m_database.open()) {
			m_lastError = "Unable to create new database file.";
			return;
		}
		if (!this->createDatabase()) {
			m_lastError = "An error has occurred creating a new database.";
			return;
		}
	}

	m_lastError = "";
	m_active = true;
}

Database::~Database()
{
	if (!m_active) return;

	if (m_database.isOpen()) {
		m_database.close();
	}

	QSqlDatabase::removeDatabase(DATABASENAME);
}



QString Database::lastError() const
{
	QString message;

	if (!m_lastError.isEmpty()) {
		message += QString("%1").arg(m_lastError);
	} else {
		QSqlError error = m_database.lastError();
		if (error.isValid()) {
			message += QString("Database: %1\n").arg(error.databaseText());
			message += QString("Driver: %1").arg(error.driverText());
		} else {
			message += "Unknown error";
		}
	}

	message = qApp->translate("Database", "A database error has occurred:\n\n") + message;

	return message;
}



int Database::databaseVersion() const
{
	QSqlQuery query;
	query.exec("PRAGMA user_version");
	if (query.next()) {
		return query.value(0).toInt();
	}

	return 0;
}

bool Database::createDatabase()
{
	return this->alterDatabase("create", qApp->translate("Database", "Creating database ..."));
}

bool Database::upgradeDatabase(int oldVersion)
{
	bool result = true;

	for (int i = oldVersion; i < DATABASEVERSION; ++i) {
		QString scriptName = QString("upgrade_%1-%2").arg(i).arg(i + 1);
		result &= this->alterDatabase(scriptName, qApp->translate("Database", "Upgrading database ..."));
	}

	return result;
}

bool Database::alterDatabase(const QString &scriptName, const QString &message)
{
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
	if (!file1.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return false;
	}
	QTextStream textStream1(&file1);
	buffer = textStream1.readAll();
	file1.close();
	queries << buffer.split("\n\n", QString::SkipEmptyParts);

	progress.setMinimum(0);
	progress.setMaximum(queries.count());

	QSqlQuery query;
	int i = 0;
	foreach (QString text, queries) {
		progress.setValue(++i);
		qApp->processEvents();

		if (!query.exec(text)) {
			return false;
		}
	}

	return true;
}

}
