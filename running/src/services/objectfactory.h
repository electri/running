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

#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "../objects/event.h"
#include "../objects/eventtype.h"
#include "../objects/shoe.h"
#include "../objects/shoemaker.h"
#include "../objects/shoemodel.h"
#include "../objects/weather.h"
#include "../objects/interval.h"
#include "../objects/intervaltype.h"

namespace Services {

class ObjectFactory
{
public:
	virtual ~ObjectFactory();

	static ObjectFactory* instance();

	Objects::BaseObject *createObject(Objects::Types::Type type);
	void copyObject(Objects::BaseObject *source, Objects::BaseObject *destination);

private:
	ObjectFactory();

	static ObjectFactory* sm_instance;
};

}

#endif
