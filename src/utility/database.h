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

#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>

/*
 * Static class to perform operations on the database.
 *
 * Call init() to open or initialize a new database. Call close() to remove all
 * connections.
 */
class Database : public QObject
{
	Q_OBJECT

public:
	static bool init();
	static void close();
	static QString lastError();

private:
	static int _databaseVersion(QSqlDatabase &db);
	static bool _createDatabase(QSqlDatabase &db);
	static bool _upgradeDatabase(QSqlDatabase &db, int oldVersion);
	static bool _alterDatabase(QSqlDatabase &db, const QString &scriptName, const QString &message);

	static QString sm_lastError;
};

#endif
