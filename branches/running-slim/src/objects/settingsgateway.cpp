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

#include <QtSql>
#include "settingsgateway.h"

SettingsGateway *SettingsGateway::sm_instance = NULL;

SettingsGateway *SettingsGateway::instance()
{
	if (sm_instance == NULL) {
		sm_instance = new SettingsGateway;
	}
	return sm_instance;
}

SettingsGateway::SettingsGateway()
{
	_clear();
	_load();
}

bool SettingsGateway::update()
{
	if (_update()) {
		_load();
		return true;
	}
	return false;
}

bool SettingsGateway::_update()
{
	QString queryText = QString("UPDATE Cfg SET MondayFirstDayOfWeek = :mondayfirstdayofweek, CfgDistanceUnitId = :cfgdistanceunitid, CfgWeightUnitId = :cfgweightunitid, CfgTemperatureUnitId = :cfgtemperatureunitid, CfgCurrencyUnitId = :cfgcurrencyunitid");

	m_lastError = "";
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isValid()) {
		if (db.isOpen()) {
			QSqlQuery query(db);
			query.prepare(queryText);
			
			query.bindValue(":mondayfirstdayofweek", m_mondayFirstDayOfWeek);
			query.bindValue(":cfgcurrencyunitid", m_currencyUnit_id);
			query.bindValue(":cfgdistanceunitid", m_distanceUnit_id);
			query.bindValue(":cfgtemperatureunitid", m_temperatureUnit_id);
			query.bindValue(":cfgweightunitid", m_weightUnit_id);

			bool rc = query.exec();
			qDebug() << "[UPDATE] " << query.lastQuery();
			if (rc) {
				return true;
			}
			m_lastError = query.lastError().text();
			return false;
		}
	}

	m_lastError = "Error on connecting to database.";
	return false;
}

void SettingsGateway::_clear()
{
	m_mondayFirstDayOfWeek = false;
	m_currencyUnit_id = 0;
	m_currencyUnit_description = "";
	m_distanceUnit_id = 0;
	m_distanceUnit_description = "";
	m_temperatureUnit_id = 0;
	m_temperatureUnit_description = "";
	m_weightUnit_id = 0;
	m_weightUnit_description = "";
}

bool SettingsGateway::_load()
{
	QString queryText = QString("SELECT Cfg.MondayFirstDayOfWeek, CfgCurrencyUnit.Id AS CfgCurrencyUnit_Id, CfgCurrencyUnit.Description AS CfgCurrencyUnit_Description, CfgDistanceUnit.Id AS CfgDistanceUnit_Id, CfgDistanceUnit.Description AS CfgDistanceUnit_Description, CfgTemperatureUnit.Id AS CfgTemperatureUnit_Id, CfgTemperatureUnit.Description AS CfgTemperatureUnit_Description, CfgWeightUnit.Id AS CfgWeightUnit_Id, CfgWeightUnit.Description AS CfgWeightUnit_Description FROM Cfg JOIN CfgCurrencyUnit ON CfgCurrencyUnit.Id = Cfg.CfgCurrencyUnitId JOIN CfgDistanceUnit ON CfgDistanceUnit.Id = Cfg.CfgDistanceUnitId JOIN CfgTemperatureUnit ON CfgTemperatureUnit.Id = Cfg.CfgTemperatureUnitId JOIN CfgWeightUnit ON CfgWeightUnit.Id = Cfg.CfgWeightUnitId");

	QSqlDatabase db = QSqlDatabase::database();
	if (db.isValid()) {
		if (db.isOpen()) {
			QSqlQuery query(db);
			query.exec(queryText);
			if (query.first()) {
				QSqlRecord record = query.record();

				QString result = "[SELECT] ";
				for (int i = 0; i < record.count(); ++i) {
					if (i != 0) result += ",";
					result += QString("%1:%2").arg(record.fieldName(i)).arg(record.value(i).toString());
				}
				qDebug() << result;

				m_mondayFirstDayOfWeek = record.value("MondayFirstDayOfWeek").toBool();
				m_currencyUnit_id = record.value("CfgCurrencyUnit_Id").toInt();
				m_currencyUnit_description = record.value("CfgCurrencyUnit_Description").toString();
				m_distanceUnit_id = record.value("CfgDistanceUnit_Id").toInt();
				m_distanceUnit_description = record.value("CfgDistanceUnit_Description").toString();
				m_temperatureUnit_id = record.value("CfgTemperatureUnit_Id").toInt();
				m_temperatureUnit_description = record.value("CfgTemperatureUnit_Description").toString();
				m_weightUnit_id = record.value("CfgWeightUnit_Id").toInt();
				m_weightUnit_description = record.value("CfgWeightUnit_Description").toString();

				return true;
			}
		}
	}

	return true;
}
