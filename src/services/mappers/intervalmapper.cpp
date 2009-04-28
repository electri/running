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

#include "intervalmapper.h"

#include "../../objects/interval.h"
#include "../../objects/intervaltype.h"
#include "../../objects/event.h"

namespace Mappers {

IntervalMapper::IntervalMapper(Services::ObjectRepository *repository)
	: BaseObjectMapper(repository)
{
	m_table = "Interval";
	m_order = "Id";
	m_columnList = "EventId, IntervalTypeId, Distance, Duration, Notes";
	m_valueList = ":eventid, :intervaltypeid, :distance, :duration, :notes";
	m_parentColumn = "EventId";
}



void IntervalMapper::setValuesFromFields(Objects::BaseObject *object, QSqlQuery &query)
{
	Objects::Interval *interval = static_cast<Objects::Interval *>(object);

	Objects::IntervalType *intervalType = static_cast<Objects::IntervalType *>(m_repository->getChild(Objects::Types::IntervalType, query.record().value("IntervalTypeId").toInt()));

	interval->setIntervalType(intervalType ? intervalType : 0);
	interval->setDistance(query.record().value("Distance").toDouble());
	interval->setDuration(query.record().value("Duration").toTime());
	interval->setNotes(query.record().value("Notes").toString());
}

void IntervalMapper::setFieldsFromValues(Objects::BaseObject *object, QSqlQuery &query)
{
	Objects::Interval *interval = static_cast<Objects::Interval *>(object);

	query.bindValue(":eventid", interval->parent() ? interval->parent()->id() : 0);
	query.bindValue(":intervaltypeid", interval->intervalType() ? interval->intervalType()->id() : 0);
	query.bindValue(":distance", interval->distance());
	query.bindValue(":duration", interval->duration());
	query.bindValue(":notes", interval->notes());
}

}
