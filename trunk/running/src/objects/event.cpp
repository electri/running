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

#include <QtDebug>

#include "event.h"

#include "eventtype.h"
#include "shoe.h"
#include "weather.h"
#include "interval.h"

#include "../utility/utility.h"

namespace Objects {

Event::Event()
{
	m_start = QDateTime();
	m_name = "";
	m_description = "";
	m_eventType = NULL;
	m_distance = 0.0;
	m_duration = QTime();
	m_notes = "";
	m_shoe = NULL;
	m_vote = 0;
	m_quality = 0;
	m_effort = 0;
	m_weight = 0;
	m_weather = NULL;
	m_temperature = 0;
}

Event::~Event()
{
}



void Event::setStart(const QDateTime &value)
{
	if (m_start != value) {
		m_start = value;
		modified();
	}
}

void Event::setName(const QString &value)
{
	if (m_name != value) {
		m_name = value;
		modified();
	}
}

void Event::setDescription(const QString &value)
{
	if (m_description != value) {
		m_description = value;
		modified();
	}
}

void Event::setEventType(EventType *value)
{
	if (m_eventType != value) {
		m_eventType = value;
		modified();
	}
}

void Event::setDistance(qreal value)
{
	if (m_distance != value) {
		m_distance = value;
		modified();
	}
}

void Event::setDuration(const QTime &value)
{
	if (m_duration != value) {
		m_duration = value;
		modified();
	}
}

void Event::setNotes(const QString &value)
{
	if (m_notes != value) {
		m_notes = value;
		modified();
	}
}

void Event::setShoe(Shoe *value)
{
	if (m_shoe != value) {
		m_shoe = value;
		modified();
	}
}

void Event::setVote(quint8 value)
{
	if (m_vote != value) {
		m_vote = value;
		modified();
	}
}

void Event::setQuality(quint8 value)
{
	if (m_quality != value) {
		m_quality = value;
		modified();
	}
}

void Event::setEffort(quint8 value)
{
	if (m_effort != value) {
		m_effort = value;
		modified();
	}
}

void Event::setWeight(qreal value)
{
	if (m_weight != value) {
		m_weight = value;
		modified();
	}
}

void Event::setWeather(Weather *value)
{
	if (m_weather != value) {
		m_weather = value;
		modified();
	}
}

void Event::setTemperature(qreal value)
{
	if (m_temperature != value) {
		m_temperature = value;
		modified();
	}
}

void Event::addInterval(Interval *value)
{
	if (!m_intervals.contains(value)) {
		m_intervals.append(value);
		modified();
	}
}

void Event::removeInterval(Interval *value)
{
	if (m_intervals.contains(value)) {
		m_intervals.removeAll(value);
		modified();
	}
}

void Event::clearIntervals()
{
	if (m_intervals.size() > 0) {
		m_intervals.clear();
		modified();
	}
}



QTime Event::paceTime() const
{
	return Utility::paceTime(m_distance, m_duration);
}

qreal Event::paceSpeed() const
{
	return Utility::paceSpeed(m_distance, m_duration);
}



QList<Objects::BaseObject *> Event::children() const
{
	QList<BaseObject *> list;
	list << m_eventType << m_shoe << m_weather;
	QList<Interval *>::const_iterator it = m_intervals.constBegin();
	while (it != m_intervals.constEnd()) { list << *it++; }
	return list;
}

QString Event::toString() const
{
	QString baseObject = static_cast<const BaseObject *>(this)->toString();
	QString eventType = m_eventType ? m_eventType->toString() : "";
	QString shoe = m_shoe ? m_shoe->toString() : "";
	QString weather = m_weather ? m_weather->toString() : "";
	QString s = QString("Event{%1|%2|%3|%4|%5|%6|%7|%8|%9|%10|%11|%12|%13}")
		.arg(baseObject).arg(m_start.date().toString()).arg(m_name).arg(m_description)
		.arg(eventType).arg(m_distance).arg(m_duration.toString()).arg(m_notes)
		.arg(shoe).arg(m_vote).arg(m_weight).arg(weather).arg(m_temperature);
	return s;
}

QDebug operator<<(QDebug dbg, const Event &object)
{
	dbg.nospace() << object.toString();
	return dbg.nospace();
}

}
