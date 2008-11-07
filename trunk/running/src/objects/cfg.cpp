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

#include "cfg.h"

namespace Objects {

Cfg::Cfg()
{
	m_mondayFirstDayOfWeek = false;
	m_cfgDistanceUnit = NULL;
	m_cfgWeightUnit = NULL;
	m_cfgTemperatureUnit = NULL;
	m_cfgCurrencyUnit = NULL;
}

Cfg::~Cfg()
{
}



void Cfg::setMondayFirstDayOfWeek(bool value)
{
	if (m_mondayFirstDayOfWeek != value) {
		m_mondayFirstDayOfWeek = value;
		modified();
	}
}

void Cfg::setCfgDistanceUnit(BaseObject *value)
{
	CfgDistanceUnit *object = static_cast<CfgDistanceUnit *>(value);

	if (m_cfgDistanceUnit != object) {
		m_cfgDistanceUnit = object;
		modified();
	}
}

void Cfg::setCfgWeightUnit(BaseObject *value)
{
	CfgWeightUnit *object = static_cast<CfgWeightUnit *>(value);

	if (m_cfgWeightUnit != object) {
		m_cfgWeightUnit = object;
		modified();
	}
}

void Cfg::setCfgTemperatureUnit(BaseObject *value)
{
	CfgTemperatureUnit *object = static_cast<CfgTemperatureUnit *>(value);

	if (m_cfgTemperatureUnit != object) {
		m_cfgTemperatureUnit = object;
		modified();
	}
}

void Cfg::setCfgCurrencyUnit(BaseObject *value)
{
	CfgCurrencyUnit *object = static_cast<CfgCurrencyUnit *>(value);

	if (m_cfgCurrencyUnit != object) {
		m_cfgCurrencyUnit = object;
		modified();
	}
}



QList<Objects::BaseObject *> Cfg::children() const
{
	QList<Objects::BaseObject *> list;
	list << m_cfgDistanceUnit << m_cfgWeightUnit << m_cfgTemperatureUnit << m_cfgCurrencyUnit;
	return list;
}

}
