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

#include <QDebug>

#include "baseobject.h"

#include "objects.h"

namespace Objects {

BaseObject::BaseObject()
{
	m_id = quint32();
	m_state = States::Created;
}

void BaseObject::modified()
{
	if (m_state == States::Selected) {
		m_state = States::Modified;
	}
}

Objects::BaseObject *BaseObject::parent() const
{
	return NULL;
}

void BaseObject::setParent(Objects::BaseObject *)
{
}

QList<Objects::BaseObject *> BaseObject::children() const
{
	return QList<Objects::BaseObject *>();
}



QString BaseObject::toString() const
{
	QString s = QString("%1(%2|%3)")
		.arg(Types::TypeNames[this->type()]).arg(m_id)
		.arg(States::StateNames[this->state()]);

	switch (this->type()) {
		case Objects::Types::Event:
		{
			const Event *object = static_cast<const Event *>(this);
			s += QString("start:%1|name:%2|description:%3|eventType:%4|distance:%5|duration:%6" \
						"|notes:%7|shoe:%8|vote:%9|weight:%10|weather:%11|temperature:%12")
				.arg(object->start().toString()).arg(object->name()).arg(object->description())
				.arg(object->eventType() ? object->eventType()->id() : 0).arg(object->distance())
				.arg(object->duration().toString()).arg(object->notes())
				.arg(object->shoe() ? object->shoe()->id() : 0).arg(object->vote()).arg(object->weight())
				.arg(object->weather() ? object->weather()->id() : 0).arg(object->temperature());
		}
		break;
		case Objects::Types::EventType:
		{
			const EventType *object = static_cast<const EventType *>(this);
			s += QString("hasMedal:%1|hasIntervals:%2").arg(object->hasMedal()).arg(object->hasIntervals());
		}
		break;
		case Objects::Types::Shoe:
		{
			const Shoe *object = static_cast<const Shoe *>(this);
			s += QString("shoeModel:%1|size:%2|purchaseDate:%3|price:%4|initialDistance:%5" \
						"|retired:%6|notes:%7}")
				.arg(object->shoeModel() ? object->shoeModel()->id() : 0).arg(object->size())
				.arg(object->purchaseDate().toString()).arg(object->price()).arg(object->initialDistance())
				.arg(object->retired()).arg(object->notes());
		}
		break;
		case Objects::Types::ShoeMaker:
		{
			const ShoeMaker *object = static_cast<const ShoeMaker *>(this);
			s += QString("description:%1").arg(object->description());
		}
		break;
		case Objects::Types::ShoeModel:
		{
			const ShoeModel *object = static_cast<const ShoeModel *>(this);
			s += QString("description:%1|shoeMaker:%2").arg(object->description())
				.arg(object->shoeMaker() ? object->shoeMaker()->id() : 0);
		}
		break;
		case Objects::Types::Weather:
		{
			const Weather *object = static_cast<const Weather *>(this);
			s += QString("description:%1").arg(object->description());
		}
		break;
		case Objects::Types::Interval:
		{
			const Interval *object = static_cast<const Interval *>(this);
			s += QString("parent:%1|intervalType:%2|distance:%3|duration:%4|notes:%5")
				.arg(object->parent() ? object->parent()->id() : 0)
				.arg(object->intervalType() ? object->intervalType()->id() : 0)
				.arg(object->distance()).arg(object->duration().toString()).arg(object->notes());
		}
		break;
		case Objects::Types::IntervalType:
		{
			const IntervalType *object = static_cast<const IntervalType *>(this);
			s += QString("description:%1").arg(object->description());
		}
		break;
		case Objects::Types::Cfg:
		{
			const Cfg *object = static_cast<const Cfg *>(this);
			s += QString("cfgDistanceUnit:%1|cfgWeightUnit:%2|cfgTemperatureUnit:%3|cfgCurrencyUnit:%4")
				.arg(object->cfgDistanceUnit() ? object->cfgDistanceUnit()->id() : 0)
				.arg(object->cfgWeightUnit() ? object->cfgWeightUnit()->id() : 0)
				.arg(object->cfgTemperatureUnit() ? object->cfgTemperatureUnit()->id() : 0)
				.arg(object->cfgCurrencyUnit() ? object->cfgCurrencyUnit()->id() : 0);
		}
		break;
		case Objects::Types::CfgDistanceUnit:
		{
			const CfgDistanceUnit *object = static_cast<const CfgDistanceUnit *>(this);
			s += QString("description:%1").arg(object->description());
		}
		break;
		case Objects::Types::CfgWeightUnit:
		{
			const CfgWeightUnit *object = static_cast<const CfgWeightUnit *>(this);
			s += QString("description:%1").arg(object->description());
		}
		break;
		case Objects::Types::CfgTemperatureUnit:
		{
			const CfgTemperatureUnit *object = static_cast<const CfgTemperatureUnit *>(this);
			s += QString("description:%1").arg(object->description());
		}
		break;
		case Objects::Types::CfgCurrencyUnit:
		{
			const CfgCurrencyUnit *object = static_cast<const CfgCurrencyUnit *>(this);
			s += QString("description:%1").arg(object->description());
		}
		break;
	}

	return s;
}

QDebug operator<<(QDebug dbg, const BaseObject &object)
{
	dbg.nospace() << object.toString();
	return dbg.space();
}

}
