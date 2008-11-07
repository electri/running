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

#include <QDebug>

#include "objectmap.h"

#include "../application.h"
#include "objectfactory.h"

namespace Services {

ObjectMap::ObjectMap()
{
	m_dynamicMemory = false;
}

ObjectMap::~ObjectMap()
{
	foreach (Objects::BaseObject *object, m_map.keys()) {
		delete object;
	}
	m_map.clear();
}



Objects::BaseObject *ObjectMap::createObject(Objects::Types::Type type)
{
	Objects::BaseObject *object = Services::ObjectFactory::instance()->createObject(type);
	updateObjectReference(object, 1, false);
	return object;
}

void ObjectMap::discardObject(Objects::BaseObject *object)
{
	updateObjectReference(object, -1, true);
}

void ObjectMap::discardObjects(QList<Objects::BaseObject *> list)
{
	foreach (Objects::BaseObject *object, list) {
		discardObject(object);
	}
}

QList<Objects::BaseObject *> ObjectMap::getAllObjects(Objects::Types::Type type)
{
	QList<Objects::BaseObject *> list;
	QList<quint32> idList = APP->objectRepository()->selectIdList(type);
	foreach (quint32 id, idList) {
		Objects::BaseObject *object = getObjectById(type, id);
		if (object) {
			list.append(object);
		}
	}
	return list;
}

QList<Objects::BaseObject *> ObjectMap::getObjectsByParent(Objects::Types::Type type, Objects::BaseObject *parent)
{
	QList<Objects::BaseObject *> list;
	QList<quint32> idList = APP->objectRepository()->selectIdList(type, parent);
	foreach (quint32 id, idList) {
		Objects::BaseObject *object = getObjectById(type, id);
		if (object) {
			object->setParent(parent);
			list.append(object);
		}
	}
	return list;
}

Objects::BaseObject *ObjectMap::getObjectById(Objects::Types::Type type, quint32 id)
{
	Objects::BaseObject *object;

	object = findObjectInCache(type, id);
	if (object) {
		updateObjectReference(object, 1, true);
		return object;
	}

	object = Services::ObjectFactory::instance()->createObject(type);
	if (object) {
		if (APP->objectRepository()->selectObject(object, id)) {
			updateObjectReference(object, 1, false);
			return object;
		}
	}

	delete object;
	return NULL;
}

bool ObjectMap::saveObject(Objects::BaseObject *object)
{
	if (object) {
		switch(object->state()) {
		case Objects::States::Selected:
			return true;
		case Objects::States::Created:
			return APP->objectRepository()->insertObject(object);
		case Objects::States::Modified:
			return APP->objectRepository()->updateObject(object);
		case Objects::States::Deleted:
			return false;
		}
	}
	return false;
}

bool ObjectMap::deleteObject(Objects::BaseObject *object)
{
	if (object) {
		switch(object->state()) {
		case Objects::States::Created:
			return true;
		case Objects::States::Selected:
		case Objects::States::Modified:
			if (m_map.contains(object)) {
				if (m_map[object] > 1) return false;
			}
			return APP->objectRepository()->deleteObject(object);
		case Objects::States::Deleted:
			return false;
		}
	}
	return false;
}

QList<Objects::BaseObject *> ObjectMap::getEventsByDate(const QDate &start, const QDate &end)
{
	QList<Objects::BaseObject *> list;
	QList<quint32> idList = APP->objectRepository()->selectEventIdListByDate(start, end);
	foreach (quint32 id, idList) {
		Objects::BaseObject *object = getObjectById(Objects::Types::Event, id);
		if (object) {
			list.append(object);
		}
	}
	return list;
}

void ObjectMap::setDynamicMemory(bool value)
{
	m_dynamicMemory = value;
}

void ObjectMap::free()
{
	QList<Objects::BaseObject *> list = m_map.keys(0);
	foreach (Objects::BaseObject *object, list) {
		m_map.remove(object);
		delete object;
	}
}

Objects::BaseObject *ObjectMap::findObjectInCache(Objects::Types::Type type, quint32 id)
{
	foreach(Objects::BaseObject *object, m_map.keys()) {
		if (object->type() == type) {
			if (object->id() == id) {
				return object;
			}
		}
	}
	return NULL;
}

void ObjectMap::updateObjectReference(Objects::BaseObject *object, qint32 count, bool recursive)
{
	if ((object == NULL) || (count == 0)) return;

#ifdef QT_DEBUG
	qDebug() << QString("ObjectMap: %1 ref:%2 recursive:%3").arg(object->toString()).arg(count).arg(recursive);
#endif

	if (m_map.contains(object)) {
		if (count < 0) {
			if (m_map[object] == 0) return;
		}

		m_map[object] += count;
	} else {
		if (count < 0) {
			return;
		}

		m_map.insert(object, 1);
	}

	if (recursive) {
		if (object->parent()) {
			updateObjectReference(object->parent(), count, false);
		}

		foreach (Objects::BaseObject *child, object->children()) {
			updateObjectReference(child, count, true);
		}
	}

	if (object->state() == Objects::States::Created) {
		if (m_map[object] == 0) {
			m_map.remove(object);
			delete object;
		}
	}

	if (m_dynamicMemory) {
		if (m_map[object] == 0) {
			m_map.remove(object);
			delete object;
		}
	}
}

QString ObjectMap::toString()
{
	quint32 oe, oet, os, osma, osmo, ow, oi, oit;
	oe = oet = os = osma = osmo = ow = oi = oit = 0;
	foreach (Objects::BaseObject *object, m_map.keys()) {
		quint32 refcount = m_map[object];
		switch (object->type()) {
			case Objects::Types::Event:			oe += refcount;		break;
			case Objects::Types::EventType:		oet += refcount;	break;
			case Objects::Types::Shoe:			os += refcount;		break;
			case Objects::Types::ShoeMaker:		osma += refcount;	break;
			case Objects::Types::ShoeModel:		osmo += refcount;	break;
			case Objects::Types::Weather:		ow += refcount;		break;
			case Objects::Types::Interval:		oi += refcount;		break;
			case Objects::Types::IntervalType:	oit += refcount;	break;
			case Objects::Types::Cfg:					break;
			case Objects::Types::CfgDistanceUnit:		break;
			case Objects::Types::CfgWeightUnit:			break;
			case Objects::Types::CfgTemperatureUnit:	break;
			case Objects::Types::CfgCurrencyUnit:		break;
		}
	}
	QString refs = QString("E%1|ET%2|S%3|SMA%4|SMO%5|W%6|I%7|IT%8")
						.arg(oe).arg(oet).arg(os).arg(osma).arg(osmo).arg(ow).arg(oi).arg(oit);
	return QString("ObjectMap: objects:%1 refs:%2.").arg(m_map.count()).arg(refs);
}

}
