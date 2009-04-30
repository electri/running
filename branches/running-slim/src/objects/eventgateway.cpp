/****************************************************************************

	running - A small program to keep track of your workouts.
	Copyright (C) 2009  Marco Gasparetto (markgabbahey@gmail.com)

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
#include "eventgateway.h"

EventGateway::EventGateway()
{
	clear();
}

void EventGateway::clear()
{
	m_id = 0;
	m_start = QDateTime();
	m_name = "";
	m_description = "";
	m_eventType_id = 0;
	m_eventType_description = "";
	m_eventType_hasMedal = false;
	m_distance = 0.0;
	m_duration = QTime();
	m_notes = "";
	m_shoe_id = 0;
	m_shoe_description = "";
	m_vote = 0;
	m_quality = 0;
	m_effort = 0;
	m_weight = 0;
	m_weather_id = 0;
	m_weather_description = "";
	m_temperature = 0;

	m_lastError = "";
}

bool EventGateway::save()
{
	return m_id == 0 ? _insert() : _update();
}

bool EventGateway::remove()
{
	return _delete();
}

bool EventGateway::_insert()
{
	QString queryText = QString("INSERT INTO Event (Start, Name, Description, EventTypeId, Distance, Duration, Notes, ShoeId, Vote, Quality, Effort, Weight, WeatherId, Temperature) VALUES (:start, :name, :description, :eventtypeid, :distance, :duration, :notes, :shoeid, :vote, :quality, :effort, :weight, :weatherid, :temperature)");

	m_lastError = "";
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isValid()) {
		if (db.isOpen()) {
			QSqlQuery query(db);
			query.prepare(queryText);

			query.bindValue(":start", this->start());
			query.bindValue(":name", this->name());
			query.bindValue(":description", this->description());
			query.bindValue(":eventtypeid", this->eventType_id());
			query.bindValue(":distance", this->distance());
			query.bindValue(":duration", this->duration());
			query.bindValue(":notes", this->notes());
			query.bindValue(":shoeid", this->shoe_id());
			query.bindValue(":vote", this->vote());
			query.bindValue(":quality", this->quality());
			query.bindValue(":effort", this->effort());
			query.bindValue(":weight", this->weight());
			query.bindValue(":weatherid", this->weather_id());
			query.bindValue(":temperature", this->temperature());
			
			bool rc = query.exec();
			qDebug() << "[INSERT] " << query.lastQuery();
			if (rc) {
				m_id = query.lastInsertId().toInt();
				return true;
			}
			m_lastError = query.lastError().text();
			return false;
		}
	}

	m_lastError = "Error on connecting to database.";
	return false;
}

bool EventGateway::_update()
{
	QString queryText = QString("UPDATE Event SET Start = :start, Name = :name, Description = :description, EventTypeId = :eventtypeid, Distance = :distance, Duration = :duration, Notes = :notes, ShoeId = :shoeid, Vote = :vote, Quality = :quality, Effort = :effort, Weight = :weight, WeatherId = :weatherid, Temperature = :temperature WHERE Id = :id");

	m_lastError = "";
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isValid()) {
		if (db.isOpen()) {
			QSqlQuery query(db);
			query.prepare(queryText);
			
			query.bindValue(":id", this->id());
			query.bindValue(":start", this->start());
			query.bindValue(":name", this->name());
			query.bindValue(":description", this->description());
			query.bindValue(":eventtypeid", this->eventType_id());
			query.bindValue(":distance", this->distance());
			query.bindValue(":duration", this->duration());
			query.bindValue(":notes", this->notes());
			query.bindValue(":shoeid", this->shoe_id());
			query.bindValue(":vote", this->vote());
			query.bindValue(":quality", this->quality());
			query.bindValue(":effort", this->effort());
			query.bindValue(":weight", this->weight());
			query.bindValue(":weatherid", this->weather_id());
			query.bindValue(":temperature", this->temperature());

			bool rc = query.exec();
			qDebug() << "[UPDATE] " << query.lastQuery();
			if (rc) {
				return true;
			}
			m_lastError = query.lastError().text();
			return false;
		}
	}

	m_lastError = "Error on connecting to database.";
	return false;
}

bool EventGateway::_delete()
{
	QString queryText = QString("DELETE FROM Event WHERE Id = %1").arg(m_id);

	m_lastError = "";
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isValid()) {
		if (db.isOpen()) {
			QSqlQuery query(db);
			bool rc = query.exec(queryText);
			qDebug() << "[DELETE] " << queryText;
			if (rc) {
				return true;
			}
			m_lastError = query.lastError().text();
			return false;
		}
	}

	m_lastError = "Error on connecting to database.";
	return false;
}

bool EventGateway::_load(const QSqlRecord &record)
{
	clear();

	m_id = record.value("Id").toInt();
	m_start = record.value("Start").toDateTime();
	m_name = record.value("Name").toString();
	m_description = record.value("Description").toString();
	m_eventType_id = record.value("EventType_Id").toInt();
	m_eventType_description = record.value("EventType_Description").toString();
	m_eventType_hasMedal = record.value("EventType_HasMedal").toBool();
	m_distance = record.value("Distance").toDouble();
	m_duration = record.value("Duration").toTime();
	m_notes = record.value("Notes").toString();
	m_shoe_id = record.value("Shoe_Id").toInt();
	m_shoe_description = record.value("Shoe_Description").toString();
	m_vote = record.value("Vote").toInt();
	m_quality = record.value("Quality").toInt();
	m_effort = record.value("Effort").toInt();
	m_weight = record.value("Weight").toInt();
	m_weather_id = record.value("Weather_Id").toInt();
	m_weather_description = record.value("Weather_Description").toString();
	m_temperature = record.value("Temperature").toInt();

	return true;
}
