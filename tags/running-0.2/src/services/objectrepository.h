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
#include <QSqlDatabase>
#include <QMap>
#include <QList>
#include <QDate>

#include "../objects/baseobject.h"

namespace Mappers { class BaseObjectMapper; }

namespace Services {

class IObjectMap;

class ObjectRepository
{
public:
	ObjectRepository();
	virtual ~ObjectRepository();

	QString lastError() const;

	/* object's non-SQL calls (common to all objects) */
	Objects::BaseObject *createObject(Objects::Types::Type type);
	void copyObject(Objects::BaseObject *src, Objects::BaseObject *dst);

	/* object's SQL calls (common to all objects) */
	QList<quint32> getIdList(Objects::Types::Type type);
	QList<quint32> getIdList(Objects::Types::Type type, Objects::BaseObject *parent);
	bool loadObject(Objects::BaseObject *, quint32 id);
	bool saveObject(Objects::BaseObject *);
	bool eraseObject(Objects::BaseObject *);

	bool updateObjects(QList<Objects::BaseObject *> objectListToSave, QList<Objects::BaseObject *> objectListToErase);

	/* object's specific SQL calls */
	QList<quint32> getEventIdListByDate(const QDate &start, const QDate &end);

	/* internally used (by the mappers) */
	Objects::BaseObject *getChild(Objects::Types::Type type, quint32 id);
	QList<Objects::BaseObject *> getCollection(Objects::Types::Type type, Objects::BaseObject *parent);

	/* external implementations (by the objectmap) */
	void setObjectMap(IObjectMap *);

private:
	bool internalSaveObject(Objects::BaseObject *);
	bool internalEraseObject(Objects::BaseObject *);

	QSqlDatabase m_database;
	QString m_lastError;
	QMap<Objects::Types::Type, Mappers::BaseObjectMapper *> m_mappers;

	IObjectMap *m_objectMap;
};

}

#endif
