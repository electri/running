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

#ifndef OBJECTMAPINTERFACE_H
#define OBJECTMAPINTERFACE_H

#include "../objects/baseobject.h"

namespace Services {

class IObjectMap
{
public:
	virtual ~IObjectMap() {}

	virtual Objects::BaseObject *getObjectById(Objects::Types::Type type, quint32 id) = 0;
	virtual QList<Objects::BaseObject *> getObjectsByParent(Objects::Types::Type type, Objects::BaseObject *parent) = 0;
	virtual void discardObject(Objects::BaseObject *) = 0;
};

}

#endif // OBJECTMAPINTERFACE_H
