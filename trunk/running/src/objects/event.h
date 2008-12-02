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

#ifndef EVENT_H
#define EVENT_H

#include <QDateTime>

#include "baseobject.h"

namespace Services { class ObjectFactory; class ObjectRepository; }

namespace Objects {

class EventType;
class Shoe;
class Weather;
class Interval;

class Event : public BaseObject
{
friend class Services::ObjectFactory;
friend class Services::ObjectRepository;

public:
	QDateTime start() const { return m_start; }
	void setStart(const QDateTime &);

	QString name() const { return m_name; }
	void setName(const QString &);

	QString description() const { return m_description; }
	void setDescription(const QString &);

	EventType *eventType() const { return m_eventType; }
	void setEventType(EventType *);

	qreal distance() const { return m_distance; }
	void setDistance(qreal);

	QTime duration() const { return m_duration; }
	void setDuration(const QTime &);

	QString notes() const { return m_notes; }
	void setNotes(const QString &);

	Shoe *shoe() const { return m_shoe; }
	void setShoe(Shoe *);

	quint8 vote() const { return m_vote; }
	void setVote(quint8);

	quint8 quality() const { return m_quality; }
	void setQuality(quint8);

	quint8 effort() const { return m_effort; }
	void setEffort(quint8);

	qreal weight() const { return m_weight; }
	void setWeight(qreal);

	Weather *weather() const { return m_weather; }
	void setWeather(Weather *);

	qreal temperature() const { return m_temperature; }
	void setTemperature(qreal);

	QList<BaseObject *> intervals() const { return m_intervals; }
	void addInterval(Interval *);
	void removeInterval(Interval *);
	void clearIntervals();

	QTime paceTime() const;
	qreal paceSpeed() const;

	Types::Type type() const { return Types::Event; }

private:
	Event();
	virtual ~Event();

	QList<Objects::BaseObject *> children() const;
	QList<Objects::BaseObject *> collectionItems() const;

	QDateTime m_start;
	QString m_name;
	QString m_description;
	EventType *m_eventType;
	qreal m_distance;
	QTime m_duration;
	QString m_notes;
	Shoe *m_shoe;
	quint8 m_vote;
	quint8 m_quality;
	quint8 m_effort;
	qreal m_weight;
	Weather *m_weather;
	qreal m_temperature;
	QList<BaseObject *> m_intervals;
};

}

#endif
