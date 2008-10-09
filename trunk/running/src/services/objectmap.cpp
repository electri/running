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

#include "objectfactory.h"
#include "objectrepository.h"

namespace Services {

ObjectMap* ObjectMap::sm_instance = NULL;

ObjectMap::ObjectMap()
{
	m_dynamicMemory = false;
//	m_map = QMap<Objects::BaseObject *, quint32>();
}

ObjectMap::~ObjectMap()
{
	QMap<Objects::BaseObject *, quint32>::iterator it = m_map.begin();
	while (it != m_map.end()) {
		delete it.key();
		++it;
	}
	m_map.clear();
}

ObjectMap* ObjectMap::instance()
{
	if (sm_instance == NULL) {
		sm_instance = new ObjectMap();
	}
	return sm_instance;
}



Objects::BaseObject *ObjectMap::createObject(Objects::Types::Type type)
{
	Objects::BaseObject *object = Services::ObjectFactory::instance()->createObject(type);
	m_map.insert(object, 1);
	return object;
}

void ObjectMap::discardObject(Objects::BaseObject *object)
{
	updateObjectReference(object, -1);
}

void ObjectMap::discardObjects(QList<Objects::BaseObject *> list)
{
	QList<Objects::BaseObject *>::const_iterator it = list.constBegin();
	while (it != list.constEnd()) {
		discardObject(*it++);
	}
}

QList<Objects::BaseObject *> ObjectMap::getAllObjects(Objects::Types::Type type)
{
	QList<Objects::BaseObject *> list;
	QList<quint32> idList = Services::ObjectRepository::instance()->selectIdList(type);
	QList<quint32>::const_iterator it = idList.constBegin();
	while (it != idList.constEnd()) {
		Objects::BaseObject *object = getObjectById(type, *it++);
		if (object) {
			list.append(object);
		}
	}
	return list;
}

QList<Objects::BaseObject *> ObjectMap::getObjectsByParent(Objects::Types::Type type, Objects::BaseObject *parent)
{
	QList<Objects::BaseObject *> list;
	QList<quint32> idList = Services::ObjectRepository::instance()->selectIdList(type, parent);
	QList<quint32>::const_iterator it = idList.constBegin();
	while (it != idList.constEnd()) {
		Objects::BaseObject *object = getObjectById(type, *it++);
		if (object) {
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
		updateObjectReference(object, 1);
		return object;
	}
	object = Services::ObjectFactory::instance()->createObject(type);
	if (object) {
		if (Services::ObjectRepository::instance()->selectObject(object, id)) {
			m_map.insert(object, 1);
			return object;
		}
		delete object;
	}
	return NULL;
}

Objects::BaseObject *ObjectMap::getObjectById(Objects::BaseObject *object, Objects::Types::Type type, quint32 id)
{
	if (!object) {
		return this->getObjectById(type, id);
	}

	if (object->id() == id) {
		return object;
	}

	this->discardObject(object);
	return this->getObjectById(type, id);
}

bool ObjectMap::saveObject(Objects::BaseObject *object)
{
	if (object) {
		switch(object->state()) {
		case Objects::States::Selected:
			return true;
		case Objects::States::Created:
			return Services::ObjectRepository::instance()->insertObject(object);
		case Objects::States::Modified:
			return Services::ObjectRepository::instance()->updateObject(object);
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
			return Services::ObjectRepository::instance()->deleteObject(object);
		case Objects::States::Deleted:
			return false;
		}
	}
	return false;
}

QList<Objects::BaseObject *> ObjectMap::getEventsByDate(const QDate &start, const QDate &end)
{
	QList<Objects::BaseObject *> list;
	QList<quint32> idList = Services::ObjectRepository::instance()->selectEventIdListByDate(start, end);
	QList<quint32>::const_iterator it = idList.constBegin();
	while (it != idList.constEnd()) {
		Objects::BaseObject *object = getObjectById(Objects::Types::Event, *it++);
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
	QList<Objects::BaseObject *>::iterator it = list.begin();
	while (it != list.end()) {
		m_map.remove(*it);
		delete *it++;
	}
}

Objects::BaseObject *ObjectMap::findObjectInCache(Objects::Types::Type type, quint32 id)
{
	QMap<Objects::BaseObject *, quint32>::const_iterator it = m_map.constBegin();
	while (it != m_map.constEnd()) {
		Objects::BaseObject *object = it.key();
		if ((object->type() == type) && (object->id() == id)) {
			return object;
		}
		++it;
	}
	return NULL;
}

void ObjectMap::updateObjectReference(Objects::BaseObject *object, qint32 count)
{
	if (object) {
		if (m_map.contains(object)) {
			if (count < 0) {
				if (m_map[object] == 0) return;
			}

			m_map[object] += count;

			if (m_dynamicMemory) {
				if (m_map[object] == 0) {
					m_map.remove(object);
					delete object;
				}
			}

			QList<Objects::BaseObject *> list = object->children();
			QList<Objects::BaseObject *>::const_iterator it = list.constBegin();
			while (it != list.constEnd()) {
				updateObjectReference(*it++, count);
			}
		}
	}
}

QString ObjectMap::toString()
{
	quint32 oe, oet, os, osma, osmo, ow, oi, oit;
	oe = oet = os = osma = osmo = ow = oi = oit = 0;
	QMap<Objects::BaseObject *, quint32>::const_iterator it = m_map.constBegin();
	while (it != m_map.constEnd()) {
		Objects::BaseObject *baseObject = it.key();
		switch (baseObject->type()) {
			case Objects::Types::Event:			oe += it.value();		break;
			case Objects::Types::EventType:		oet += it.value();		break;
			case Objects::Types::Shoe:			os += it.value();		break;
			case Objects::Types::ShoeMaker:		osma += it.value();		break;
			case Objects::Types::ShoeModel:		osmo += it.value();		break;
			case Objects::Types::Weather:		ow += it.value();		break;
			case Objects::Types::Interval:		oi += it.value();		break;
			case Objects::Types::IntervalType:	oit += it.value();		break;
		}
		++it;
	}
	QString refs = QString("E%1|ET%2|S%3|SMA%4|SMO%5|W%6|I%7|IT%8")
						.arg(oe).arg(oet).arg(os).arg(osma).arg(osmo).arg(ow).arg(oi).arg(oit);
	return QString("ObjectMap: objects: %1, refs: %2.").arg(m_map.count()).arg(refs);
}

}
