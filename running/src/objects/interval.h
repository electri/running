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

#ifndef INTERVAL_H
#define INTERVAL_H

#include <QDateTime>

#include "baseobject.h"

namespace Services { class ObjectFactory; }

namespace Objects {

class IntervalType;
class Event;

class Interval : public BaseObject
{
friend class Services::ObjectFactory;

public:
	IntervalType *intervalType() const { return m_intervalType; }
	void setIntervalType(BaseObject *);

	qreal distance() const { return m_distance; }
	void setDistance(qreal);

	QTime duration() const { return m_duration; }
	void setDuration(const QTime &);

	QString notes() const { return m_notes; }
	void setNotes(const QString &);

	QTime paceTime() const;
	qreal paceSpeed() const;



	Types::Type type() const { return Types::Interval; }

	Objects::BaseObject *parent() const;
	void setParent(Objects::BaseObject *);
	QList<Objects::BaseObject *> children() const;

private:
	Interval();
	virtual ~Interval();

	Event *m_event;
	IntervalType *m_intervalType;
	qreal m_distance;
	QTime m_duration;
	QString m_notes;
};

}

#endif
