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

#ifndef CFG_H
#define CFG_H

#include "comboobject.h"
#include "cfgdistanceunit.h"
#include "cfgweightunit.h"
#include "cfgtemperatureunit.h"
#include "cfgcurrencyunit.h"

namespace Services { class ObjectFactory; }

namespace Objects {

class Cfg : public BaseObject
{
friend class Services::ObjectFactory;

public:
	virtual ~Cfg();

	bool isMondayFirstDayOfWeek() const { return m_mondayFirstDayOfWeek; }
	void setMondayFirstDayOfWeek(bool);

	CfgDistanceUnit *cfgDistanceUnit() const { return m_cfgDistanceUnit; }
	void setCfgDistanceUnit(BaseObject *);

	CfgWeightUnit *cfgWeightUnit() const { return m_cfgWeightUnit; }
	void setCfgWeightUnit(BaseObject *);

	CfgTemperatureUnit *cfgTemperatureUnit() const { return m_cfgTemperatureUnit; }
	void setCfgTemperatureUnit(BaseObject *);

	CfgCurrencyUnit *cfgCurrencyUnit() const { return m_cfgCurrencyUnit; }
	void setCfgCurrencyUnit(BaseObject *);

	Types::Type type() const { return Types::Cfg; }

	QList<Objects::BaseObject *> children() const;

private:
	Cfg();

	bool m_mondayFirstDayOfWeek;
	CfgDistanceUnit *m_cfgDistanceUnit;
	CfgWeightUnit *m_cfgWeightUnit;
	CfgTemperatureUnit *m_cfgTemperatureUnit;
	CfgCurrencyUnit *m_cfgCurrencyUnit;
};

}

#endif
