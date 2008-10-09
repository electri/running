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

#include "statisticsservice.h"

#include "../utility/utility.h"

namespace Utility {

StatisticsService* StatisticsService::sm_instance = NULL;

StatisticsService::StatisticsService()
{
}

StatisticsService::~StatisticsService()
{
}

StatisticsService* StatisticsService::instance()
{
	if (sm_instance == NULL) {
		sm_instance = new StatisticsService();
	}
	return sm_instance;
}



QList<StatisticsResults::EventsPerDate> StatisticsService::eventsPerDate()
{
	QList<StatisticsResults::EventsPerDate> list;

	QSqlQuery query;
	QString s;

	s = "select " \
		"0,0,count(id),sum(distance), " \
		"datetime('1970-01-01',sum(strftime('%s','1970-01-01T'||duration))||' seconds') " \
		"from event";
	query.exec(s);
	while (query.next()) {
		list << newEventResult(query.record());
	}

	s = "select " \
		"strftime('%Y',start),0,count(id),sum(distance), " \
		"datetime('1970-01-01',sum(strftime('%s','1970-01-01T'||duration))||' seconds') " \
		"from event group by strftime('%Y',start) order by strftime('%Y',start)";
	query.exec(s);
	while (query.next()) {
		list << newEventResult(query.record());
	}

	s = "select " \
		"strftime('%Y',start),strftime('%m',start),count(id),sum(distance), " \
		"datetime('1970-01-01',sum(strftime('%s','1970-01-01T'||duration))||' seconds') " \
		"from event group by strftime('%Y%m',start) order by strftime('%Y%m',start)";
	query.exec(s);
	while (query.next()) {
		list << newEventResult(query.record());
	}

	return list;
}

StatisticsResults::EventsPerDate StatisticsService::event(int year, int month)
{
	StatisticsResults::EventsPerDate row;

	QSqlQuery query;
	QString s;

	s = "select " \
		+ QString("%1").arg(year) + "," + QString("%1").arg(month) + ",count(id),sum(distance), " \
		"datetime('1970-01-01',sum(strftime('%s','1970-01-01T'||duration))||' seconds') " \
		"from event";
	if (month > 0) {
		s += QString(" where start like '%1-%2%%'").arg(year).arg(month, 2, 10, QChar('0'));
	} else if (year > 0) {
		s += QString(" where start like '%1%%'").arg(year);
	}
	query.exec(s);
	if (query.next()) {
		row = newEventResult(query.record());
	}

	return row;
}

QList<StatisticsResults::Shoes> StatisticsService::shoes()
{
	QList<StatisticsResults::Shoes> list;

	QString s = "select " \
		"s.id,s.description,count(e.id),sum(e.distance), " \
		"datetime('1970-01-01',sum(strftime('%s','1970-01-01T'||e.duration))||' seconds'), " \
		"s.initialdistance,s.price,s.retired " \
		"from shoeview s " \
		"left join event e on e.shoeid = s.id " \
		"group by s.id order by s.id";
	QSqlQuery query;
	query.exec(s);
	while (query.next()) {
		list << newShoeResult(query.record());
	}

	return list;
}

StatisticsResults::Shoes StatisticsService::shoe(int id)
{
	StatisticsResults::Shoes row;

	QString s = "select " \
		"s.id,s.description,count(e.id),sum(e.distance), " \
		"datetime('1970-01-01',sum(strftime('%s','1970-01-01T'||e.duration))||' seconds'), " \
		"s.initialdistance,s.price,s.retired " \
		"from shoeview s " \
		"left join event e on e.shoeid = s.id";
	s += QString(" where s.id = %1").arg(id);
	QSqlQuery query;
	query.exec(s);
	if (query.first()) {
		row = newShoeResult(query.record());
	}

	return row;
}



StatisticsResults::EventsPerDate StatisticsService::newEventResult(const QSqlRecord &record)
{
	StatisticsResults::EventsPerDate r;

	if (!record.isEmpty()) {
		r.year = record.value(0).toInt();
		r.month = record.value(1).toInt();
		r.events = record.value(2).toInt();
		r.distance = record.value(3).toDouble();
		r.duration = record.value(4).toDateTime();
		if (r.distance > 0) {
			r.paceTime = Utility::paceTime(r.distance, r.duration);
			r.paceSpeed = Utility::paceSpeed(r.distance, r.duration);
		}
	}

	return r;
}

StatisticsResults::Shoes StatisticsService::newShoeResult(const QSqlRecord &record)
{
	StatisticsResults::Shoes r;

	if (!record.isEmpty()) {
		r.id = record.value(0).toInt();
		r.description = record.value(1).toString();
		r.events = record.value(2).toInt();
		r.distance = record.value(3).toDouble();
		r.duration = record.value(4).toDateTime();
		r.initialDistance = record.value(5).toDouble();
		r.price = record.value(6).toDouble();
		r.retired = record.value(7).toBool();
		double distance = r.distance + r.initialDistance;
		if (distance > 0) {
			r.pricePerDistance = r.price / distance;
		}
	}

	return r;
}

}
