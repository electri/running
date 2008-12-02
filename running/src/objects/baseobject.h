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

#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <QObject>
#include <QString>

namespace Services { class ObjectRepository; class ObjectMap; }
namespace Mappers { class BaseObjectMapper; }

namespace Objects {

namespace Types {
	enum Type {
		Event, EventType, Shoe, ShoeMaker, ShoeModel, Weather, Interval, IntervalType,
		Cfg, CfgDistanceUnit, CfgWeightUnit, CfgTemperatureUnit, CfgCurrencyUnit
	};
	const QString TypeNames[] = {
		"Event", "EventType", "Shoe", "ShoeMaker", "ShoeModel", "Weather", "Interval", "IntervalType",
		"Cfg", "CfgDistanceUnit", "CfgWeightUnit", "CfgTemperatureUnit", "CfgCurrencyUnit"
	};
}

namespace States {
	enum State { Selected, Created, Modified, Deleted };
	const QString StateNames[] = { "Sel", "Cre", "Mod", "Del" };
}

class BaseObject
{
friend class Services::ObjectRepository;
friend class Services::ObjectMap;
friend class Mappers::BaseObjectMapper;

public:
	virtual ~BaseObject() {}

	quint32 id() const { return m_id; }

	States::State state() const { return m_state; }

	virtual Types::Type type() const = 0;

	virtual Objects::BaseObject *parent() const;
	virtual void setParent(Objects::BaseObject *parent);

	QString toString() const;

protected:
	BaseObject();

	void modified();

	virtual QList<Objects::BaseObject *> children() const;
	virtual QList<Objects::BaseObject *> collectionItems() const;

	quint32 m_id;
	States::State m_state;

	QList<BaseObject *> m_removedCollectionItems;
};

QDebug operator<<(QDebug, const BaseObject &);

}

#endif
