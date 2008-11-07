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

#include "eventtypemapper.h"

#include "../../objects/eventtype.h"

namespace Mappers {

EventTypeMapper::EventTypeMapper()
	: ComboObjectMapper("EventType")
{
	m_order = "Description";
	m_columnList = "Description, HasMedal, HasIntervals";
	m_valueList = ":description, :hasmedal, :hasintervals";
}



void EventTypeMapper::setValuesFromFields(Objects::BaseObject *object, QSqlQuery &query)
{
	Objects::EventType *eventType = static_cast<Objects::EventType *>(object);

	eventType->setDescription(query.record().value("Description").toString());

	eventType->setMedal(query.record().value("HasMedal").toBool());
	eventType->setIntervals(query.record().value("HasIntervals").toBool());
}

void EventTypeMapper::setFieldsFromValues(Objects::BaseObject *object, QSqlQuery &query)
{
	Objects::EventType *eventType = static_cast<Objects::EventType *>(object);

	query.bindValue(":description", eventType->description());

	query.bindValue(":hasmedal", eventType->hasMedal());
	query.bindValue(":hasintervals", eventType->hasIntervals());
}

}
