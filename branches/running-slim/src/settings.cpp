/****************************************************************************

	running - A small program to keep track of your workouts.
	Copyright (C) 2009  Marco Gasparetto (markgabbahey@gmail.com)

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

#include "settings.h"

Settings *Settings::sm_instance = 0 /*NULL*/;// NULL is not defined , wait for nullptr in C++0x

Settings *Settings::instance()
{
        if (!sm_instance) {
		sm_instance = new Settings;
	}
	return sm_instance;
}

Settings::Settings()
{
	m_settings = new QSettings();

	if (!m_settings->contains("General/Toolbar Icon Size"))
#ifdef Q_WS_MAC
		m_settings->setValue("General/Toolbar Icon Size", 48);
#else
		m_settings->setValue("General/Toolbar Icon Size", 24);
#endif

	if (!m_settings->contains("General/Toolbar Tool Button Style"))
#ifdef Q_WS_MAC
		m_settings->setValue("General/Toolbar Tool Button Style", Qt::ToolButtonTextUnderIcon);
#else
		m_settings->setValue("General/Toolbar Tool Button Style", Qt::ToolButtonTextBesideIcon);
#endif

	if (!m_settings->contains("Calendar/Monday is first day of week"))
		m_settings->setValue("Calendar/Monday is first day of week", false);

	if (!m_settings->contains("Units of measurement/Currency Unit"))
		m_settings->setValue("Units of measurement/Currency Unit", "$");
	if (!m_settings->contains("Units of measurement/Distance Unit"))
		m_settings->setValue("Units of measurement/Distance Unit", "Mi");
	if (!m_settings->contains("Units of measurement/Temperature Unit"))
		m_settings->setValue("Units of measurement/Temperature Unit", "°F");
	if (!m_settings->contains("Units of measurement/Weight Unit"))
		m_settings->setValue("Units of measurement/Weight Unit", "Li");
}

Settings::~Settings()
{
	delete m_settings;
        m_settings = 0;
}

int Settings::toolbarIconSize() const
{
	return m_settings->value("General/Toolbar Icon Size", 24).toInt();
}

Qt::ToolButtonStyle Settings::toolbarToolButtonStyle() const
{
	int toolButtonStyle = m_settings->value("General/Toolbar Tool Button Style", Qt::ToolButtonTextBesideIcon).toInt();
	return static_cast<Qt::ToolButtonStyle>(toolButtonStyle);
}

bool Settings::isMondayFirstDayOfWeek() const
{
	return m_settings->value("Calendar/Monday is first day of week", false).toBool();
}

QString Settings::currencyUnit() const
{
	return m_settings->value("Units of measurement/Currency Unit", "$").toString();
}

QString Settings::distanceUnit() const
{
	return m_settings->value("Units of measurement/Distance Unit", "Mi").toString();
}

QString Settings::temperatureUnit() const
{
	return m_settings->value("Units of measurement/Temperature Unit", "°F").toString();
}

QString Settings::weightUnit() const
{
	return m_settings->value("Units of measurement/Weight Unit", "Li").toString();
}
