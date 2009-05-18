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

#ifndef EVENTGATEWAY_H
#define EVENTGATEWAY_H

#include <QString>
#include <QDateTime>
#include <QSqlRecord>

class EventGateway
{
friend class EventFinder;

public:
	EventGateway();

	int id() const { return m_id; }
	void setStart(const QDateTime& value) { m_start = value; }
	QDateTime start() const { return m_start; }
	void setName(const QString& value) { m_name = value; }
	QString name() const { return m_name; }
	void setDescription(const QString& value) { m_description = value; }
	QString description() const { return m_description; }
	void setEventType_id(int value) { m_eventType_id = value; }
	int eventType_id() const { return m_eventType_id; }
	QString eventType_description() const { return m_eventType_description; }
	bool eventType_hasMedal() const { return m_eventType_hasMedal; }
	void setDistance(qreal value) { m_distance = value; }
	qreal distance() const { return m_distance; }
	void setDuration(const QTime& value) { m_duration = value; }
	QTime duration() const { return m_duration; }
	void setNotes(const QString& value) { m_notes = value; }
	QString notes() const { return m_notes; }
	void setShoe_id(int value) { m_shoe_id = value; }
	int shoe_id() const { return m_shoe_id; }
	QString shoe_description() const { return m_shoe_description; }
	void setVote(quint8 value) { m_vote = value; }
	quint8 vote() const { return m_vote; }
	void setQuality(quint8 value) { m_quality = value; }
	quint8 quality() const { return m_quality; }
	void setEffort(quint8 value) { m_effort = value; }
	quint8 effort() const { return m_effort; }
	void setWeight(qreal value) { m_weight = value; }
	qreal weight() const { return m_weight; }
	void setWeather_id(int value) { m_weather_id = value; }
	int weather_id() const { return m_weather_id; }
	QString weather_description() const { return m_weather_description; }
	void setTemperature(qreal value) { m_temperature = value; }
	qreal temperature() const { return m_temperature; }

	void clear();
	bool save();
	bool remove();

	QString lastError() const { return m_lastError; }

private:
	void _load(const QSqlRecord &record);
	bool _insert();
	bool _update();
	bool _delete();

	int m_id;
	QDateTime m_start;
	QString m_name;
	QString m_description;
	int m_eventType_id;
	QString m_eventType_description;
	bool m_eventType_hasMedal;
	qreal m_distance;
	QTime m_duration;
	QString m_notes;
	int m_shoe_id;
	QString m_shoe_description;
	quint8 m_vote;
	quint8 m_quality;
	quint8 m_effort;
	qreal m_weight;
	int m_weather_id;
	QString m_weather_description;
	qreal m_temperature;

	QString m_lastError;
};

#endif // EVENTGATEWAY_H
