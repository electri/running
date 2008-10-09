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

#ifndef OBJECTREPOSITORY_H
#define OBJECTREPOSITORY_H

#include <QString>
#include <QList>
#include <QSqlDatabase>
#include <QDate>

#include "../objects/baseobject.h"

namespace Mappers { class BaseObjectMapper; }

namespace Services {

class ObjectRepository
{
public:
	virtual ~ObjectRepository();

	static ObjectRepository* instance();

	bool isActive() const { return m_active; }
	QString lastError() const;

	/* generic SQL calls (transactions) */
	bool transaction();
	bool commit();
	bool rollback();

	/* object's SQL calls (common to all objects) */
	QList<quint32> selectIdList(Objects::Types::Type type);
	QList<quint32> selectIdList(Objects::Types::Type type, Objects::BaseObject *parent);
	bool selectObject(Objects::BaseObject *, quint32 id);
	bool insertObject(Objects::BaseObject *);
	bool updateObject(Objects::BaseObject *);
	bool deleteObject(Objects::BaseObject *);

	/* object's specific SQL calls */
	QList<quint32> selectEventIdListByDate(const QDate &start, const QDate &end);

private:
	ObjectRepository();

	static ObjectRepository* sm_instance;

	bool m_active;
	QSqlDatabase m_database;
	QString m_lastError;
	QMap<Objects::Types::Type, Mappers::BaseObjectMapper *> m_mappers;
};

}

#endif
