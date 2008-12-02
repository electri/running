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

#include "objectrepository.h"

#include "objectmapinterface.h"
#include "objectfactory.h"
#include "transaction.h"
#include "mappers/cfgmapper.h"
#include "mappers/comboobjectmapper.h"
#include "mappers/intervalmapper.h"
#include "mappers/eventmapper.h"
#include "mappers/eventtypemapper.h"
#include "mappers/shoemapper.h"
#include "mappers/shoemodelmapper.h"

namespace Services {

ObjectRepository::ObjectRepository()
{
	m_database = QSqlDatabase::database();

	m_mappers.insert(Objects::Types::Cfg, new Mappers::CfgMapper(this));
	m_mappers.insert(Objects::Types::CfgDistanceUnit, new Mappers::ComboObjectMapper(this, "CfgDistanceUnit"));
	m_mappers.insert(Objects::Types::CfgWeightUnit, new Mappers::ComboObjectMapper(this, "CfgWeightUnit"));
	m_mappers.insert(Objects::Types::CfgTemperatureUnit, new Mappers::ComboObjectMapper(this, "CfgTemperatureUnit"));
	m_mappers.insert(Objects::Types::CfgCurrencyUnit, new Mappers::ComboObjectMapper(this, "CfgCurrencyUnit"));
	m_mappers.insert(Objects::Types::Event, new Mappers::EventMapper(this));
	m_mappers.insert(Objects::Types::EventType, new Mappers::EventTypeMapper(this));
	m_mappers.insert(Objects::Types::Interval, new Mappers::IntervalMapper(this));
	m_mappers.insert(Objects::Types::IntervalType, new Mappers::ComboObjectMapper(this, "IntervalType"));
	m_mappers.insert(Objects::Types::Shoe, new Mappers::ShoeMapper(this));
	m_mappers.insert(Objects::Types::ShoeMaker, new Mappers::ComboObjectMapper(this, "ShoeMaker"));
	m_mappers.insert(Objects::Types::ShoeModel, new Mappers::ShoeModelMapper(this));
	m_mappers.insert(Objects::Types::Weather, new Mappers::ComboObjectMapper(this, "Weather"));

	m_lastError = "";

	m_objectMap = NULL;
}

ObjectRepository::~ObjectRepository()
{
	QMap<Objects::Types::Type, Mappers::BaseObjectMapper *>::iterator it = m_mappers.begin();
	while (it != m_mappers.end()) {
		delete it.value();
		++it;
	}
	m_mappers.clear();
}



QString ObjectRepository::lastError() const
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

//	message = qApp->translate("ObjectRepository", "A database error has occurred:\n\n") + message;

	return message;
}



Objects::BaseObject *ObjectRepository::createObject(Objects::Types::Type type)
{
#ifdef OBJECTREPOSITORY_DEBUG
	qDebug() << QString("ObjectRepository: Creating a new %1.").arg(Objects::Types::TypeNames[type]);
#endif

	return ObjectFactory::createObject(type);
}

void ObjectRepository::copyObject(Objects::BaseObject *src, Objects::BaseObject *dst)
{
	return ObjectFactory::copyObject(src, dst);
}



QList<quint32> ObjectRepository::getIdList(Objects::Types::Type type)
{
#ifdef OBJECTREPOSITORY_DEBUG
	qDebug() << QString("ObjectRepository: Getting all indexes of %1.").arg(Objects::Types::TypeNames[type]);
#endif

	QList<quint32> list;
	list = m_mappers[type]->selectIdList();
	m_lastError = m_mappers[type]->lastError();
	return list;
}

QList<quint32> ObjectRepository::getIdList(Objects::Types::Type type, Objects::BaseObject *parent)
{
	QList<quint32> list;
	list = m_mappers[type]->selectIdList(parent);
	m_lastError = m_mappers[type]->lastError();
	return list;
}

bool ObjectRepository::loadObject(Objects::BaseObject *object, quint32 id)
{
#ifdef OBJECTREPOSITORY_DEBUG
	qDebug() << QString("ObjectRepository: Loading %1 with id=%2.").arg(object->toString()).arg(id);
#endif

	if (object) {
		if (object->state() == Objects::States::Deleted) {
			m_lastError = "Error on loading a deleted object.";
			return false;
		}

		if(!m_mappers[object->type()]->selectObject(object, id)) {
			m_lastError = m_mappers[object->type()]->lastError();
			return false;
		}
		m_lastError = "";

		return true;
	}

	return false;
}

bool ObjectRepository::saveObject(Objects::BaseObject *object)
{
#ifdef OBJECTREPOSITORY_DEBUG
	qDebug() << QString("ObjectRepository: Saving %1.").arg(object->toString());
#endif

	Services::Transaction transaction;

	if (!transaction.transaction()) {
		m_lastError = transaction.lastError();
		return false;
	}

	if (!this->internalSaveObject(object)) {
		transaction.rollback();
		return false;
	}
	transaction.commit();

	return true;
}

bool ObjectRepository::eraseObject(Objects::BaseObject *object)
{
#ifdef OBJECTREPOSITORY_DEBUG
	qDebug() << QString("ObjectRepository: Erasing %1.").arg(object->toString());
#endif

	Services::Transaction transaction;

	if (!transaction.transaction()) {
		m_lastError = transaction.lastError();
		return false;
	}

	if (!this->internalEraseObject(object)) {
		transaction.rollback();
		return false;
	}
	transaction.commit();

	return true;
}



bool ObjectRepository::updateObjects(QList<Objects::BaseObject *> objectListToSave, QList<Objects::BaseObject *> objectListToErase)
{
	Services::Transaction::Transaction transaction;

	if (!transaction.transaction()) {
		return false;
	}

	foreach (Objects::BaseObject *object, objectListToSave) {
		if (!this->internalSaveObject(object)) {
			transaction.rollback();
			return false;
		}
	}
	foreach (Objects::BaseObject *object, objectListToErase) {
		if (!this->internalEraseObject(object)) {
			transaction.rollback();
			return false;
		}
	}
	transaction.commit();

	return true;
}



QList<quint32> ObjectRepository::getEventIdListByDate(const QDate &start, const QDate &end)
{
	QList<quint32> list;
	if (m_mappers.contains(Objects::Types::Event)) {
		Mappers::EventMapper *eventMapper = static_cast<Mappers::EventMapper *>(m_mappers[Objects::Types::Event]);
		list = eventMapper->selectIdListByDate(start, end);
		m_lastError = m_mappers[Objects::Types::Event]->lastError();
	}
	return list;
}



Objects::BaseObject *ObjectRepository::getChild(Objects::Types::Type type, quint32 id /*, Objects::BaseObject *old_child*/)
{
	if (id == 0) {
		return NULL;
	}

	/*if (old_child) {
		if (old_child->id() == id) {
			return old_child;
		} else {
			if (m_objectMap) {
				m_objectMap->discardObject(old_child);
			}
		}
	}*/

	Objects::BaseObject *child = NULL;

	if (m_objectMap) {
		child = m_objectMap->getObjectById(type, id);
	}

	if (child == NULL) {
		child = this->createObject(type);
		if (!this->loadObject(child, id)) {
			child = NULL;
		}
	}

	return child;
}

QList<Objects::BaseObject *> ObjectRepository::getCollection(Objects::Types::Type type, Objects::BaseObject *parent)
{
	QList<Objects::BaseObject *> list;

	if (parent != NULL) {
		if (m_objectMap) {
//			list = m_objectMap->getObjectById(type, id);
		} else {
			QList<quint32> idList = getIdList(type, parent);
			foreach (quint32 id, idList) {
				Objects::BaseObject *item = createObject(type);
				loadObject(item, id);
				list << item;
			}
		}
	}

	return list;
}

void ObjectRepository::setObjectMap(IObjectMap *objectMap)
{
	m_objectMap = objectMap;
}



bool ObjectRepository::internalSaveObject(Objects::BaseObject *object)
{
	if (object) {
		if (object->state() == Objects::States::Deleted) {
			m_lastError = "Error on saving a deleted object.";
			return false;
		}

		foreach (Objects::BaseObject *child, object->children()) {
			if (child != NULL) {
				if (!this->internalSaveObject(child)) {
					return false;
				}
			}
		}

		if (object->state() == Objects::States::Created) {
			if (!m_mappers[object->type()]->insertObject(object)) {
				m_lastError = m_mappers[object->type()]->lastError();
				return false;
			}
		}
		if (object->state() == Objects::States::Modified) {
			if (!m_mappers[object->type()]->updateObject(object)) {
				m_lastError = m_mappers[object->type()]->lastError();
				return false;
			}
		}
		m_lastError = "";

		foreach (Objects::BaseObject *item, object->collectionItems()) {
			if (item != NULL) {
				if (!this->internalSaveObject(item)) {
					return false;
				}
			}
		}

		foreach (Objects::BaseObject *item, object->m_removedCollectionItems) {
			if (item != NULL) {
				if (!this->internalEraseObject(item)) {
					return false;
				}
			}
		}
		object->m_removedCollectionItems.clear();

		return true;
	}

	return false;
}

bool ObjectRepository::internalEraseObject(Objects::BaseObject *object)
{
	if (object) {
		if (object->state() == Objects::States::Deleted) {
			m_lastError = "Error on erasing a deleted object.";
			return false;
		}

		foreach (Objects::BaseObject *collectionItem, object->collectionItems()) {
			if (collectionItem != NULL) {
				if (!this->internalEraseObject(collectionItem)) {
					return false;
				}
			}
		}

		foreach (Objects::BaseObject *item, object->m_removedCollectionItems) {
			if (item != NULL) {
				if (!this->internalEraseObject(item)) {
					return false;
				}
			}
		}
		object->m_removedCollectionItems.clear();

		if(!m_mappers[object->type()]->deleteObject(object)) {
			m_lastError = m_mappers[object->type()]->lastError();
			return false;
		}
		m_lastError = "";

		return true;
	}

	return false;
}

}
