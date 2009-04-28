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

#ifndef CONFIGURATIONSERVICE_H
#define CONFIGURATIONSERVICE_H

#include <QMap>
#include <QList>
#include <QDate>
#include <QString>

#include "../objects/baseobject.h"

namespace Services {

class Configuration
{
public:
	virtual ~Configuration();

	static Configuration* instance();

	Objects::BaseObject *createObject(Objects::Types::Type type);
	void discardObject(Objects::BaseObject *);
	void discardObjects(QList<Objects::BaseObject *>);
	QList<Objects::BaseObject *> getAllObjects(Objects::Types::Type type);
	QList<Objects::BaseObject *> getObjectsByParent(Objects::Types::Type type, Objects::BaseObject *parent);
	Objects::BaseObject *getObjectById(Objects::Types::Type type, quint32 id);
	bool saveObject(Objects::BaseObject *);
	bool deleteObject(Objects::BaseObject *);

	QList<Objects::BaseObject *> getEventsByDate(const QDate &start, const QDate &end);

	void setDynamicMemory(bool value);
	void free();

	QString toString();

private:
	Configuration();

	Objects::BaseObject *findObjectInCache(Objects::Types::Type type, quint32 id);
	void updateObjectReference(Objects::BaseObject *, qint32 count, bool recursive);

	static Configuration* sm_instance;

	bool m_dynamicMemory;
	QMap<Objects::BaseObject *, quint32> m_map;
};

}

#endif
