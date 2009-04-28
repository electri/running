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

#ifndef OBJECTMAPSERVICE_H
#define OBJECTMAPSERVICE_H

#include <QMap>
#include <QList>
#include <QDate>
#include <QString>

#include "../objects/baseobject.h"
#include "objectmapinterface.h"

namespace Services {

class Database;
class ObjectRepository;

class ObjectMap : public IObjectMap
{
public:
	ObjectMap();
	virtual ~ObjectMap();

	bool isActive() const;
	QString lastError() const;

	Objects::BaseObject *createObject(Objects::Types::Type type);
	void registerObject(Objects::BaseObject *);
	void discardObject(Objects::BaseObject *);
	void discardObjects(QList<Objects::BaseObject *>);
	QList<Objects::BaseObject *> getAllObjects(Objects::Types::Type type);
	QList<Objects::BaseObject *> getObjectsByParent(Objects::Types::Type type, Objects::BaseObject *parent);
	Objects::BaseObject *getObjectById(Objects::Types::Type type, quint32 id);
	bool saveObject(Objects::BaseObject *);
	bool eraseObject(Objects::BaseObject *);

	bool updateObjects(QList<Objects::BaseObject *> objectListToSave, QList<Objects::BaseObject *> objectListToErase);

	QList<Objects::BaseObject *> getEventsByDate(const QDate &start, const QDate &end);

	void setDynamicMemory(bool value);
	void free();

	int objCount();
	int refCount(Objects::Types::Type);
	QString toString();

private:
	Objects::BaseObject *findObjectInCache(Objects::Types::Type type, quint32 id);
	void updateObjectReference(Objects::BaseObject *, qint32 count, bool recursive);

	bool m_dynamicMemory;
	QMap<Objects::BaseObject *, quint32> m_map;

	Services::Database *m_database;
	Services::ObjectRepository *m_objectRepository;
};

}

#endif
