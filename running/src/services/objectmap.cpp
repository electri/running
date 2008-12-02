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

#include "../services/database.h"
#include "../services/objectrepository.h"

namespace Services {

ObjectMap::ObjectMap()
{
	m_dynamicMemory = false;

	m_database = new Services::Database();

	m_objectRepository = new Services::ObjectRepository();
	m_objectRepository->setObjectMap(this);
}

ObjectMap::~ObjectMap()
{
	foreach (Objects::BaseObject *object, m_map.keys()) {
		delete object;
	}
	m_map.clear();

	delete m_objectRepository;
	m_objectRepository = NULL;

	delete m_database;
	m_database = NULL;
}



bool ObjectMap::isActive() const
{
	return m_database->isActive();
}

QString ObjectMap::lastError() const
{
	return m_objectRepository->lastError();
}



Objects::BaseObject *ObjectMap::createObject(Objects::Types::Type type)
{
#ifdef OBJECTMAP_DEBUG
	qDebug() << QString("ObjectMap: Creating a new %1.").arg(Objects::Types::TypeNames[type]);
#endif

	Objects::BaseObject *object = m_objectRepository->createObject(type);
	registerObject(object);
	return object;
}

void ObjectMap::registerObject(Objects::BaseObject *object)
{
	updateObjectReference(object, 1, true);
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
#ifdef OBJECTMAP_DEBUG
	qDebug() << QString("ObjectMap: Getting all %1.").arg(Objects::Types::TypeNames[type]);
#endif

	QList<Objects::BaseObject *> list;
	QList<quint32> idList = m_objectRepository->getIdList(type);
	foreach (quint32 id, idList) {
		Objects::BaseObject *object = this->getObjectById(type, id);
		if (object) {
			list.append(object);
		}
	}
	return list;
}

QList<Objects::BaseObject *> ObjectMap::getObjectsByParent(Objects::Types::Type type, Objects::BaseObject *parent)
{
	QList<Objects::BaseObject *> list;
	QList<quint32> idList = m_objectRepository->getIdList(type, parent);
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
#ifdef OBJECTMAP_DEBUG
	qDebug() << QString("ObjectMap: Getting %1 with id=%2.").arg(Objects::Types::TypeNames[type]).arg(id);
#endif

	Objects::BaseObject *object;

	object = findObjectInCache(type, id);
	if (object) {
		updateObjectReference(object, 1, true);
		return object;
	}

	object = m_objectRepository->createObject(type);
	if (object) {
		if (m_objectRepository->loadObject(object, id)) {
			updateObjectReference(object, 1, false);
			return object;
		}
	}

	delete object;
	return NULL;
}

bool ObjectMap::saveObject(Objects::BaseObject *object)
{
#ifdef OBJECTMAP_DEBUG
	qDebug() << QString("ObjectMap: Saving %1.").arg(object->toString());
#endif

	if (object) {
//		if (m_map.contains(object)) {
			return m_objectRepository->saveObject(object);
//		}
	}
	return false;
}

bool ObjectMap::eraseObject(Objects::BaseObject *object)
{
#ifdef OBJECTMAP_DEBUG
	qDebug() << QString("ObjectMap: Erasing %1.").arg(object->toString());
#endif

	if (object) {
//		if (m_map.contains(object)) {
			return m_objectRepository->eraseObject(object);
//		}
	}
	return false;
}



bool ObjectMap::updateObjects(QList<Objects::BaseObject *> objectListToSave, QList<Objects::BaseObject *> objectListToErase)
{
	return m_objectRepository->updateObjects(objectListToSave, objectListToErase);
}



QList<Objects::BaseObject *> ObjectMap::getEventsByDate(const QDate &start, const QDate &end)
{
	QList<Objects::BaseObject *> list;
	QList<quint32> idList = m_objectRepository->getEventIdListByDate(start, end);
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
#ifdef OBJECTMAP_DEBUG
	qDebug() << QString("ObjectMap: Freeing map.");
#endif

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

#ifdef OBJECTMAP_DEBUG
	qDebug() << QString("ObjectMap: %1 ref:%2 recursive:%3.").arg(object->toString()).arg(count).arg(recursive);
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
//		if (object->parent()) {
//			updateObjectReference(object->parent(), count, false);
//		}

		foreach (Objects::BaseObject *child, object->children()) {
			updateObjectReference(child, count, true);
		}
		foreach (Objects::BaseObject *item, object->collectionItems()) {
			updateObjectReference(item, count, true);
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

int ObjectMap::objCount()
{
	return m_map.count();
}

int ObjectMap::refCount(Objects::Types::Type type)
{
	int result = 0;

	foreach (Objects::BaseObject *object, m_map.keys()) {
		if (object->type() == type) {
			result += m_map[object];
		}
	}

	return result;
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
	QString refs = QString("E%1;ET%2;S%3;SMA%4;SMO%5;W%6;I%7;IT%8")
						.arg(oe).arg(oet).arg(os).arg(osma).arg(osmo).arg(ow).arg(oi).arg(oit);
	return QString("ObjectMap: objects:%1 refs:%2.").arg(m_map.count()).arg(refs);
}

}
