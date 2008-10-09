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

#include "objectfactory.h"

namespace Services {

ObjectFactory* ObjectFactory::sm_instance = NULL;

ObjectFactory::ObjectFactory()
{
}

ObjectFactory::~ObjectFactory()
{
}

ObjectFactory* ObjectFactory::instance()
{
	if (sm_instance == NULL) {
		sm_instance = new ObjectFactory();
	}
	return sm_instance;
}



Objects::BaseObject *ObjectFactory::createObject(Objects::Types::Type type)
{
	switch (type) {
		case Objects::Types::Event:		return new Objects::Event();
		case Objects::Types::EventType:	return new Objects::EventType();
		case Objects::Types::Shoe:		return new Objects::Shoe();
		case Objects::Types::ShoeMaker:	return new Objects::ShoeMaker();
		case Objects::Types::ShoeModel:	return new Objects::ShoeModel();
		case Objects::Types::Weather:	return new Objects::Weather();
		case Objects::Types::Interval:	return new Objects::Interval();
		case Objects::Types::IntervalType:	return new Objects::IntervalType();
	}
	return NULL;
}

void ObjectFactory::copyObject(Objects::BaseObject *src, Objects::BaseObject *dst)
{
	if (src && dst) {
		if (src->type() == dst->type()) {
			switch (src->type()) {
				case Objects::Types::Event:		*static_cast<Objects::Event *>(dst) = *static_cast<Objects::Event *>(src);						break;
				case Objects::Types::EventType:	*static_cast<Objects::EventType *>(dst) = *static_cast<Objects::EventType *>(src);				break;
				case Objects::Types::Shoe:		*static_cast<Objects::Shoe *>(dst) = *static_cast<Objects::Shoe *>(src);						break;
				case Objects::Types::ShoeMaker:	*static_cast<Objects::ShoeMaker *>(dst) = *static_cast<Objects::ShoeMaker *>(src);				break;
				case Objects::Types::ShoeModel:	*static_cast<Objects::ShoeModel *>(dst) = *static_cast<Objects::ShoeModel *>(src);				break;
				case Objects::Types::Weather:	*static_cast<Objects::Weather *>(dst) = *static_cast<Objects::Weather *>(src);					break;
				case Objects::Types::Interval:	*static_cast<Objects::Interval *>(dst) = *static_cast<Objects::Interval *>(src);				break;
				case Objects::Types::IntervalType:	*static_cast<Objects::IntervalType *>(dst) = *static_cast<Objects::IntervalType *>(src);	break;
			}
		}
	}
}

}
