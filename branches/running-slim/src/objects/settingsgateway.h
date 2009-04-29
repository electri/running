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

#ifndef SETTINGSGATEWAY_H
#define SETTINGSGATEWAY_H

#include <QString>
#include <QDateTime>
#include <QSqlRecord>

class SettingsGateway
{
public:
	static SettingsGateway *instance();

	void setMondayFirstDayOfWeek(bool value) { m_mondayFirstDayOfWeek = value; }
	bool isMondayFirstDayOfWeek() const { return m_mondayFirstDayOfWeek; }
	void setCurrencyUnitId(int value) { m_currencyUnit_id = value; }
	int currencyUnit_id() const { return m_currencyUnit_id; }
	QString currencyUnit_description() const { return m_currencyUnit_description; }
	void setDistanceUnitId(int value) { m_distanceUnit_id = value; }
	int distanceUnit_id() const { return m_distanceUnit_id; }
	QString distanceUnit_description() const { return m_distanceUnit_description; }
	void setTemperatureUnitId(int value) { m_temperatureUnit_id = value; }
	int temperatureUnit_id() const { return m_temperatureUnit_id; }
	QString temperatureUnit_description() const { return m_temperatureUnit_description; }
	void setWeightUnitId(int value) { m_weightUnit_id = value; }
	int weightUnit_id() const { return m_weightUnit_id; }
	QString weightUnit_description() const { return m_weightUnit_description; }

	bool update();

	QString lastError() const { return m_lastError; }

private:
	SettingsGateway();
	static SettingsGateway *sm_instance;

	void _clear();
	bool _load();
	bool _update();

	bool m_mondayFirstDayOfWeek;
	int m_currencyUnit_id;
	QString m_currencyUnit_description;
	int m_distanceUnit_id;
	QString m_distanceUnit_description;
	int m_temperatureUnit_id;
	QString m_temperatureUnit_description;
	int m_weightUnit_id;
	QString m_weightUnit_description;

	QString m_lastError;
};

#endif // SETTINGSGATEWAY_H
