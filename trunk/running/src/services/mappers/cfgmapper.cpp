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

#include <QtSql>

#include "cfgmapper.h"

#include "../../objects/cfg.h"

namespace Mappers {

CfgMapper::CfgMapper(Services::ObjectRepository *repository)
	: BaseObjectMapper(repository)
{
	m_table = "Cfg";
	m_order = "Id";
	m_columnList = "MondayFirstDayOfWeek, CfgDistanceUnitId, CfgWeightUnitId, CfgTemperatureUnitId, CfgCurrencyUnitId";
	m_valueList = ":mondayfirstdayofweek, :cfgdistanceunitid, :cfgweightunitid, :cfgtemperatureunitid, :cfgcurrencyunitid";
}



void CfgMapper::setValuesFromFields(Objects::BaseObject *object, QSqlQuery &query)
{
	Objects::Cfg *cfg = static_cast<Objects::Cfg *>(object);

	Objects::CfgDistanceUnit *cfgDistanceUnit = static_cast<Objects::CfgDistanceUnit *>(m_repository->getChild(Objects::Types::CfgDistanceUnit, query.record().value("CfgDistanceUnitId").toInt() /*, cfg->cfgDistanceUnit()*/));
	Objects::CfgWeightUnit *cfgWeightUnit = static_cast<Objects::CfgWeightUnit *>(m_repository->getChild(Objects::Types::CfgWeightUnit, query.record().value("CfgWeightUnitId").toInt() /*, cfg->cfgWeightUnit()*/));
	Objects::CfgTemperatureUnit *cfgTemperatureUnit = static_cast<Objects::CfgTemperatureUnit *>(m_repository->getChild(Objects::Types::CfgTemperatureUnit, query.record().value("CfgTemperatureUnitId").toInt() /*, cfg->cfgTemperatureUnit()*/));
	Objects::CfgCurrencyUnit *cfgCurrencyUnit = static_cast<Objects::CfgCurrencyUnit *>(m_repository->getChild(Objects::Types::CfgCurrencyUnit, query.record().value("CfgCurrencyUnitId").toInt() /*, cfg->cfgCurrencyUnit()*/));

	cfg->setMondayFirstDayOfWeek(query.record().value("MondayFirstDayOfWeek").toBool());
	cfg->setCfgDistanceUnit(cfgDistanceUnit ? cfgDistanceUnit : 0);
	cfg->setCfgWeightUnit(cfgWeightUnit ? cfgWeightUnit : 0);
	cfg->setCfgTemperatureUnit(cfgTemperatureUnit ? cfgTemperatureUnit : 0);
	cfg->setCfgCurrencyUnit(cfgCurrencyUnit ? cfgCurrencyUnit : 0);
}

void CfgMapper::setFieldsFromValues(Objects::BaseObject *object, QSqlQuery &query)
{
	Objects::Cfg *cfg = static_cast<Objects::Cfg *>(object);

	query.bindValue(":mondayfirstdayofweek", cfg->isMondayFirstDayOfWeek());
	query.bindValue(":cfgdistanceunitid", cfg->cfgDistanceUnit() ? cfg->cfgDistanceUnit()->id() : 0);
	query.bindValue(":cfgweightunitid", cfg->cfgWeightUnit() ? cfg->cfgWeightUnit()->id() : 0);
	query.bindValue(":cfgtemperatureunitid", cfg->cfgTemperatureUnit() ? cfg->cfgTemperatureUnit()->id() : 0);
	query.bindValue(":cfgcurrencyunitid", cfg->cfgCurrencyUnit() ? cfg->cfgCurrencyUnit()->id() : 0);
}

}
