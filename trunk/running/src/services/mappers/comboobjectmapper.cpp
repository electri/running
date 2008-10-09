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

#include "comboobjectmapper.h"

#include "../../objects/comboobject.h"

namespace Mappers {

ComboObjectMapper::ComboObjectMapper(const QString &table)
	: BaseObjectMapper()
{
	m_table = table;
	m_order = "Description";
	m_columnList = "Description";
	m_valueList = ":description";
}



bool ComboObjectMapper::selectObjectByDescription(Objects::ComboObject *object, const QString &description)
{
	QString text = QString("SELECT Id FROM %1 WHERE Description = '%2'").arg(m_table).arg(description);
	if (!m_order.isEmpty()) text += QString(" ORDER BY %1").arg(m_order);
	QSqlQuery query;
	query.exec(text);
	if (query.next()) {
		object->m_id = query.value(0).toInt();
		get(object, query);
		object->m_state = Objects::States::Selected;
		m_lastError = "";
		return true;
	}
	return false;
}



void ComboObjectMapper::get(Objects::BaseObject *object, QSqlQuery &query)
{
	Objects::ComboObject *comboObject = static_cast<Objects::ComboObject *>(object);

	comboObject->setDescription(query.record().value("Description").toString());
}

void ComboObjectMapper::set(Objects::BaseObject *object, QSqlQuery &query)
{
	Objects::ComboObject *comboObject = static_cast<Objects::ComboObject *>(object);

	query.bindValue(":description", comboObject->description());
}

}
