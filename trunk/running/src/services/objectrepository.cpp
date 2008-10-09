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

#include <QtSql>
#include <QApplication>
#include <QDir>
#include <QFile>

#include "objectrepository.h"

#include "mappers/comboobjectmapper.h"
#include "mappers/eventmapper.h"
#include "mappers/eventtypemapper.h"
#include "mappers/shoemapper.h"
#include "mappers/shoemodelmapper.h"
#include "mappers/intervalmapper.h"

namespace Services {

ObjectRepository* ObjectRepository::sm_instance = NULL;

ObjectRepository::ObjectRepository()
{
	m_active = false;

	// init database
	QDir::setCurrent(QApplication::applicationDirPath());

	if (!QFile::exists("running.db3")) {
		return;
	}

	m_database = QSqlDatabase::addDatabase("QSQLITE");

	m_database.setDatabaseName("running.db3");

	if (!m_database.open()) {
		return;
	}

	// init mapper's instances
	m_mappers.insert(Objects::Types::Event, new Mappers::EventMapper());
	m_mappers.insert(Objects::Types::EventType, new Mappers::EventTypeMapper());
	m_mappers.insert(Objects::Types::Shoe, new Mappers::ShoeMapper());
	m_mappers.insert(Objects::Types::ShoeMaker, new Mappers::ComboObjectMapper("ShoeMaker"));
	m_mappers.insert(Objects::Types::ShoeModel, new Mappers::ShoeModelMapper());
	m_mappers.insert(Objects::Types::Weather, new Mappers::ComboObjectMapper("Weather"));
	m_mappers.insert(Objects::Types::Interval, new Mappers::IntervalMapper());
	m_mappers.insert(Objects::Types::IntervalType, new Mappers::ComboObjectMapper("IntervalType"));

	m_lastError = "";

	m_active = true;
}

ObjectRepository::~ObjectRepository()
{
	if (!m_active) return;

	QMap<Objects::Types::Type, Mappers::BaseObjectMapper *>::iterator it = m_mappers.begin();
	while (it != m_mappers.end()) {
		delete it.value();
		++it;
	}
	m_mappers.clear();

	if (m_database.isOpen()) {
		m_database.close();
	}

	QSqlDatabase::removeDatabase("running.db3");
}

ObjectRepository* ObjectRepository::instance()
{
	if (sm_instance == NULL) {
		sm_instance = new ObjectRepository();
	}
	return sm_instance;
}

QString ObjectRepository::lastError() const
{
	if (!m_active) return "DATABASE: No connection";

	QString message = "";

	if (!m_lastError.isEmpty()) {
		message += QString("LAST QUERY: %1\n").arg(m_lastError);
	}

	QSqlError error = m_database.lastError();

	if (error.isValid()) {
		message += QString("DATABASE: %1\n").arg(error.databaseText());
		message += QString("DRIVER: %1").arg(error.driverText());
	}

	return message.isEmpty() ? "No error" : message;
}



bool ObjectRepository::transaction()
{
	if (!m_active) return false;

	return m_database.transaction();
}

bool ObjectRepository::commit()
{
	if (!m_active) return false;

	return m_database.commit();
}

bool ObjectRepository::rollback()
{
	if (!m_active) return false;

	return m_database.rollback();
}



QList<quint32> ObjectRepository::selectIdList(Objects::Types::Type type)
{
	QList<quint32> list;
	if (m_active) {
		if (m_mappers.contains(type))
		{
			list = m_mappers[type]->selectIdList();
			m_lastError = m_mappers[type]->lastError();
		}
	}
	return list;
}

QList<quint32> ObjectRepository::selectIdList(Objects::Types::Type type, Objects::BaseObject *parent)
{
	QList<quint32> list;
	if (m_active) {
		if (m_mappers.contains(type))
		{
			list = m_mappers[type]->selectIdList(parent);
			m_lastError = m_mappers[type]->lastError();
		}
	}
	return list;
}

bool ObjectRepository::selectObject(Objects::BaseObject *object, quint32 id)
{
	bool result = false;
	if (m_active) {
		if (object) {
			if (m_mappers.contains(object->type()))
			{
				result = m_mappers[object->type()]->selectObject(object, id);
				m_lastError = m_mappers[object->type()]->lastError();
			}
		}
	}
	return result;
}

bool ObjectRepository::insertObject(Objects::BaseObject *object)
{
	bool result = false;
	if (m_active) {
		if (object) {
			if (m_mappers.contains(object->type()))
			{
				result = m_mappers[object->type()]->insertObject(object);
				m_lastError = m_mappers[object->type()]->lastError();
			}
		}
	}
	return result;
}

bool ObjectRepository::updateObject(Objects::BaseObject *object)
{
	bool result = false;
	if (m_active) {
		if (object) {
			if (m_mappers.contains(object->type()))
			{
				result = m_mappers[object->type()]->updateObject(object);
				m_lastError = m_mappers[object->type()]->lastError();
			}
		}
	}
	return result;
}

bool ObjectRepository::deleteObject(Objects::BaseObject *object)
{
	bool result = false;
	if (m_active) {
		if (object) {
			if (m_mappers.contains(object->type()))
			{
				result = m_mappers[object->type()]->deleteObject(object);
				m_lastError = m_mappers[object->type()]->lastError();
			}
		}
	}
	return result;
}

QList<quint32> ObjectRepository::selectEventIdListByDate(const QDate &start, const QDate &end)
{
	QList<quint32> list;
	if (m_active) {
		if (m_mappers.contains(Objects::Types::Event))
		{
			Mappers::EventMapper *eventMapper = static_cast<Mappers::EventMapper *>(m_mappers[Objects::Types::Event]);
			list = eventMapper->selectIdListByDate(start, end);
			m_lastError = m_mappers[Objects::Types::Event]->lastError();
		}
	}
	return list;
}

}