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

#include "transaction.h"

namespace Services {

Transaction::Transaction()
{
	m_database = QSqlDatabase::database();
	m_lastError = "";
}

Transaction::~Transaction()
{
}



QString Transaction::lastError() const
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

//	message = qApp->translate("Transaction", "A database error has occurred:\n\n") + message;

	return message;
}



bool Transaction::transaction()
{
	return m_database.transaction();
}

bool Transaction::commit()
{
	return m_database.commit();
}

bool Transaction::rollback()
{
	return m_database.rollback();
}

}
