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

#include "mappers/comboobjectmapper.h"
#include "mappers/eventmapper.h"
#include "mappers/eventtypemapper.h"
#include "mappers/shoemapper.h"
#include "mappers/shoemodelmapper.h"
#include "mappers/intervalmapper.h"
#include "mappers/cfgmapper.h"

const QString DATABASENAME = "running.db3";
const int DATABASEVERSION = 1;

namespace Services {

ObjectRepository::ObjectRepository()
{
	m_active = false;

	m_database = QSqlDatabase::addDatabase("QSQLITE");
	m_database.setDatabaseName(DATABASENAME);

	// init database
	QDir::setCurrent(QApplication::applicationDirPath());

	if (QFile::exists(DATABASENAME)) {
		if (!m_database.open()) {
			m_lastError = "Unable to open existing database file.";
			return;
		}
		int databaseVersion = this->databaseVersion();
		if (databaseVersion > DATABASEVERSION)
		{
			m_lastError = "The database file is for a newer version of the software.";
			return;
		} else if (databaseVersion < DATABASEVERSION) {
			if (!this->upgradeDatabase(databaseVersion)) {
				m_lastError = "An error has occurred upgrading the database.";
				return;
			}
		}
	} else {
		if (!m_database.open()) {
			m_lastError = "Unable to create new database file.";
			return;
		}
		if (!this->createDatabase()) {
			m_lastError = "An error has occurred creating a new database.";
			return;
		}
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
	m_mappers.insert(Objects::Types::Cfg, new Mappers::CfgMapper());
	m_mappers.insert(Objects::Types::CfgDistanceUnit, new Mappers::ComboObjectMapper("CfgDistanceUnit"));
	m_mappers.insert(Objects::Types::CfgWeightUnit, new Mappers::ComboObjectMapper("CfgWeightUnit"));
	m_mappers.insert(Objects::Types::CfgTemperatureUnit, new Mappers::ComboObjectMapper("CfgTemperatureUnit"));
	m_mappers.insert(Objects::Types::CfgCurrencyUnit, new Mappers::ComboObjectMapper("CfgCurrencyUnit"));

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

	QSqlDatabase::removeDatabase(DATABASENAME);
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

	message = qApp->translate("ObjectRepository", "A database error has occurred:\n\n") + message;

	return message;
}



int ObjectRepository::databaseVersion() const
{
	QSqlQuery query;
	query.exec("PRAGMA user_version");
	if (query.next()) {
		return query.value(0).toInt();
	}

	return 0;
}

bool ObjectRepository::createDatabase()
{
	return this->alterDatabase("create", qApp->translate("ObjectRepository", "Creating database ..."));
}

bool ObjectRepository::upgradeDatabase(int oldVersion)
{
	bool result = true;

	for (int i = oldVersion; i < DATABASEVERSION; ++i) {
		QString scriptName = QString("upgrade_%1-%2").arg(i).arg(i + 1);
		result &= this->alterDatabase(scriptName, qApp->translate("ObjectRepository", "Upgrading database ..."));
	}

	return result;
}

bool ObjectRepository::alterDatabase(const QString &scriptName, const QString &message)
{
	QProgressDialog progress;
	progress.setWindowModality(Qt::WindowModal);
	progress.setLabelText(message);

	QStringList queries;

	QFile file(QString(":/sql/%1.sql").arg(scriptName));
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return false;
	}
	QTextStream textStream(&file);
	QString buffer = textStream.readAll();
	file.close();
	queries << buffer.split("\n\n", QString::SkipEmptyParts);

	QFile file1(QString(":/sql/%1_init.sql").arg(scriptName));
	if (!file1.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return false;
	}
	QTextStream textStream1(&file1);
	buffer = textStream1.readAll();
	file1.close();
	queries << buffer.split("\n\n", QString::SkipEmptyParts);

	progress.setMinimum(0);
	progress.setMaximum(queries.count());

	QSqlQuery query;
	int i = 0;
	foreach (QString text, queries) {
		progress.setValue(++i);
		qApp->processEvents();
		
		if (!query.exec(text)) {
			return false;
		}
	}

	return true;
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
