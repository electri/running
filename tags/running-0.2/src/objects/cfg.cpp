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

void Cfg::setCfgDistanceUnit(CfgDistanceUnit *value)
{
	if (m_cfgDistanceUnit != value) {
		m_cfgDistanceUnit = value;
		modified();
	}
}

void Cfg::setCfgWeightUnit(CfgWeightUnit *value)
{
	if (m_cfgWeightUnit != value) {
		m_cfgWeightUnit = value;
		modified();
	}
}

void Cfg::setCfgTemperatureUnit(CfgTemperatureUnit *value)
{
	if (m_cfgTemperatureUnit != value) {
		m_cfgTemperatureUnit = value;
		modified();
	}
}

void Cfg::setCfgCurrencyUnit(CfgCurrencyUnit *value)
{
	if (m_cfgCurrencyUnit != value) {
		m_cfgCurrencyUnit = value;
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
