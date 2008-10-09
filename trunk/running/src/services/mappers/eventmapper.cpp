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

#include "eventmapper.h"

#include "../../objects/event.h"
#include "../../objects/eventtype.h"
#include "../../objects/shoe.h"
#include "../../objects/weather.h"
#include "../../objects/interval.h"

#include "../objectmap.h"
//#include "../objectfactory.h"
//#include "../objectrepository.h"

namespace Mappers {

EventMapper::EventMapper()
{
	m_table = "Event";
	m_order = "Id";
	m_columnList = "Start, Name, Description, EventTypeId, Distance, Duration, Notes, ShoeId, Vote, Quality, Effort, Weight, WeatherId, Temperature";
	m_valueList = ":start, :name, :description, :eventtypeid, :distance, :duration, :notes, :shoesid, :vote, :quality, :effort, :weight, :weatherid, :temperature";
}



QList<quint32> EventMapper::selectIdListByDate(const QDate &start, const QDate &end)
{
	QList<quint32> list;
	QString text = QString("SELECT Id FROM Event WHERE " \
		"CAST(strftime('%Y',Start) AS INTEGER) >= %1 AND CAST(strftime('%Y',Start) AS INTEGER) <= %2 AND " \
		"CAST(strftime('%m',Start) AS INTEGER) >= %3 AND CAST(strftime('%m',Start) AS INTEGER) <= %4 AND " \
		"CAST(strftime('%d',Start) AS INTEGER) >= %5 AND CAST(strftime('%d',Start) AS INTEGER) <= %6")
		.arg(start.year()).arg(end.year())
		.arg(start.month()).arg(end.month())
		.arg(start.day()).arg(end.day());
	QSqlQuery query;
	query.exec(text);
	while (query.next()) {
		quint32 id = query.value(0).toInt();
		list.append(id);
	}
	m_lastError = "";
	return list;
}



void EventMapper::get(Objects::BaseObject *object, QSqlQuery &query)
{
	Objects::Event *event = static_cast<Objects::Event *>(object);

	Objects::EventType *eventType = static_cast<Objects::EventType *>(
		Services::ObjectMap::instance()->getObjectById(event->eventType(), Objects::Types::EventType, query.record().value("EventTypeId").toInt()));
//	Objects::EventType *eventType = static_cast<Objects::EventType *>(
//										Services::ObjectFactory::instance()->createObject(Objects::Types::EventType));
//	Services::ObjectRepository::instance()->selectObject(eventType, query.record().value("EventTypeId").toInt());

	Objects::Shoe *shoe = static_cast<Objects::Shoe *>(
		Services::ObjectMap::instance()->getObjectById(event->shoe(), Objects::Types::Shoe, query.record().value("ShoeId").toInt()));
//	Objects::Shoe *shoe = static_cast<Objects::Shoe *>(
//										Services::ObjectFactory::instance()->createObject(Objects::Types::Shoe));
//	Services::ObjectRepository::instance()->selectObject(shoe, query.record().value("ShoeId").toInt());

	Objects::Weather *weather = static_cast<Objects::Weather *>(
		Services::ObjectMap::instance()->getObjectById(event->weather(), Objects::Types::Weather, query.record().value("WeatherId").toInt()));

	event->setStart(query.record().value("Start").toDateTime());
	event->setName(query.record().value("Name").toString());
	event->setDescription(query.record().value("Description").toString());
	event->setEventType(eventType ? eventType : 0);
	event->setDistance(query.record().value("Distance").toDouble());
	event->setDuration(query.record().value("Duration").toTime());
	event->setNotes(query.record().value("Notes").toString());
	event->setShoe(shoe ? shoe : 0);
	event->setVote(query.record().value("Vote").toInt());
	event->setQuality(query.record().value("Quality").toInt());
	event->setEffort(query.record().value("Effort").toInt());
	event->setWeight(query.record().value("Weight").toDouble());
	event->setWeather(weather ? weather : 0);
	event->setTemperature(query.record().value("Temperature").toDouble());
}

void EventMapper::set(Objects::BaseObject *object, QSqlQuery &query)
{
	Objects::Event *event = static_cast<Objects::Event *>(object);

	query.bindValue(":start", event->start());
	query.bindValue(":name", event->name());
	query.bindValue(":description", event->description());
	query.bindValue(":eventtypeid", event->eventType() ? event->eventType()->id() : 0);
	query.bindValue(":description", event->description());
	query.bindValue(":distance", event->distance());
	query.bindValue(":duration", event->duration());
	query.bindValue(":notes", event->notes());
	query.bindValue(":shoeid", event->shoe() ? event->shoe()->id() : 0);
	query.bindValue(":vote", event->vote());
	query.bindValue(":quality", event->quality());
	query.bindValue(":effort", event->effort());
	query.bindValue(":weight", event->weight());
	query.bindValue(":weatherid", event->weather() ? event->weather()->id() : 0);
	query.bindValue(":temperature", event->temperature());
}

}