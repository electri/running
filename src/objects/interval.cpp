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

#include "interval.h"

#include "intervaltype.h"
#include "event.h"

#include "../utility/utility.h"

namespace Objects {

Interval::Interval()
{
	m_event = NULL;
	m_intervalType = NULL;
	m_distance = 0.0;
	m_duration = QTime();
	m_notes = "";
}

Interval::~Interval()
{
}



void Interval::setIntervalType(IntervalType *value)
{
	if (m_intervalType != value) {
		m_intervalType = value;
		modified();
	}
}

void Interval::setDistance(qreal value)
{
	if (m_distance != value) {
		m_distance = value;
		modified();
	}
}

void Interval::setDuration(const QTime &value)
{
	if (m_duration != value) {
		m_duration = value;
		modified();
	}
}

void Interval::setNotes(const QString &value)
{
	if (m_notes != value) {
		m_notes = value;
		modified();
	}
}



QTime Interval::paceTime() const
{
	return Utility::paceTime(m_distance, m_duration);
}

qreal Interval::paceSpeed() const
{
	return Utility::paceSpeed(m_distance, m_duration);
}



Objects::BaseObject *Interval::parent() const
{
	return m_event;
}

void Interval::setParent(Objects::BaseObject *value)
{
	Event *event = static_cast<Event *>(value);

	if (m_event != event) {
		m_event = event;
		modified();
	}
}

QList<Objects::BaseObject *> Interval::children() const
{
	QList<Objects::BaseObject *> list;
	list << m_intervalType;
	return list;
}

}
