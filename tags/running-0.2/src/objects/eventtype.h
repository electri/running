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

#ifndef EVENTTYPE_H
#define EVENTTYPE_H

#include "comboobject.h"

namespace Services { class ObjectFactory; }

namespace Objects {

class EventType : public ComboObject
{
friend class Services::ObjectFactory;

public:
	bool hasMedal() const { return m_hasMedal; }
	void setMedal(bool);

	bool hasIntervals() const { return m_hasIntervals; }
	void setIntervals(bool);



	Types::Type type() const { return Types::EventType; }

private:
	EventType();
	virtual ~EventType();

	bool m_hasMedal;
	bool m_hasIntervals;
};

}

#endif
