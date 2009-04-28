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

Configuration* Configuration::sm_instance = NULL;

Configuration::Configuration()
{
	m_dynamicMemory = false;
}

Configuration::~Configuration()
{
	foreach (Objects::BaseObject *object, m_map.keys()) {
		delete object;
	}
	m_map.clear();
}

Configuration* Configuration::instance()
{
	if (sm_instance == NULL) {
		sm_instance = new Configuration();
	}
	return sm_instance;
}



Objects::BaseObject *Configuration::createObject(Objects::Types::Type type)
{
	Objects::BaseObject *object = Services::ObjectFactory::instance()->createObject(type);
	updateObjectReference(object, 1, false);
	return object;
}

void Configuration::discardObject(Objects::BaseObject *object)
{
	updateObjectReference(object, -1, true);
}

void Configuration::discardObjects(QList<Objects::BaseObject *> list)
{
	foreach (Objects::BaseObject *object, list) {
		discardObject(object);
	}
}

QList<Objects::BaseObject *> Configuration::getAllObjects(Objects::Types::Type type)
{
	QList<Objects::BaseObject *> list;
	QList<quint32> idList = Services::ObjectRepository::instance()->selectIdList(type);
	foreach (quint32 id, idList) {
		Objects::BaseObject *object = getObjectById(type, id);
		if (object) {
			list.append(object);
		}
	}
	return list;
}

QList<Objects::BaseObject *> Configuration::getObjectsByParent(Objects::Types::Type type, Objects::BaseObject *parent)
{
	QList<Objects::BaseObject *> list;
	QList<quint32> idList = Services::ObjectRepository::instance()->selectIdList(type, parent);
	foreach (quint32 id, idList) {
		Objects::BaseObject *object = getObjectById(type, id);
		if (object) {
			object->setParent(parent);
			list.append(object);
		}
	}
	return list;
}

Objects::BaseObject *Configuration::getObjectById(Objects::Types::Type type, quint32 id)
{
	Objects::BaseObject *object;

	object = findObjectInCache(type, id);
	if (object) {
		updateObjectReference(object, 1, true);
		return object;
	}

	object = Services::ObjectFactory::instance()->createObject(type);
	if (object) {
		if (Services::ObjectRepository::instance()->selectObject(object, id)) {
			updateObjectReference(object, 1, false);
			return object;
		}
	}

	delete object;
	return NULL;
}

bool Configuration::saveObject(Objects::BaseObject *object)
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

bool Configuration::deleteObject(Objects::BaseObject *object)
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

QList<Objects::BaseObject *> Configuration::getEventsByDate(const QDate &start, const QDate &end)
{
	QList<Objects::BaseObject *> list;
	QList<quint32> idList = Services::ObjectRepository::instance()->selectEventIdListByDate(start, end);
	foreach (quint32 id, idList) {
		Objects::BaseObject *object = getObjectById(Objects::Types::Event, id);
		if (object) {
			list.append(object);
		}
	}
	return list;
}

void Configuration::setDynamicMemory(bool value)
{
	m_dynamicMemory = value;
}

void Configuration::free()
{
	QList<Objects::BaseObject *> list = m_map.keys(0);
	foreach (Objects::BaseObject *object, list) {
		m_map.remove(object);
		delete object;
	}
}

Objects::BaseObject *Configuration::findObjectInCache(Objects::Types::Type type, quint32 id)
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

void Configuration::updateObjectReference(Objects::BaseObject *object, qint32 count, bool recursive)
{
	if ((object == NULL) || (count == 0)) return;

//	qDebug() << *object << count << recursive;

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

	if (m_dynamicMemory) {
		if (m_map[object] == 0) {
			m_map.remove(object);
			delete object;
		}
	}
}

QString Configuration::toString()
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
	return QString("Configuration: objects: %1, refs: %2.").arg(m_map.count()).arg(refs);
}

}
