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

#include "baseobjectmapper.h"

namespace Mappers {

BaseObjectMapper::BaseObjectMapper(Services::ObjectRepository *repository)
{
	m_repository = repository;

	m_database = QSqlDatabase::database();
	m_table = "";
	m_order = "";
	m_columnList = "";
	m_valueList = "";
	m_parentColumn = "";
	m_lastError = "";
}

BaseObjectMapper::~BaseObjectMapper()
{
}



QList<quint32> BaseObjectMapper::selectIdList()
{
	QList<quint32> list;
	QString text = QString("SELECT Id FROM %1").arg(m_table);
	if (!m_order.isEmpty()) text += QString(" ORDER BY %1").arg(m_order);
	QSqlQuery query;
	query.exec(text);
	while (query.next()) {
		quint32 id = query.value(0).toInt();
		list.append(id);
	}
	m_lastError = "";
	return list;
}

QList<quint32> BaseObjectMapper::selectIdList(Objects::BaseObject *parent)
{
	QList<quint32> list;
	if (!m_parentColumn.isEmpty() && (parent != NULL)) {
		QString text = QString("SELECT Id FROM %1").arg(m_table);
		text += QString(" WHERE %1 = %2").arg(m_parentColumn).arg(parent->id());
		if (!m_order.isEmpty()) text += QString(" ORDER BY %1").arg(m_order);
		QSqlQuery query;
		query.exec(text);
		while (query.next()) {
			quint32 id = query.value(0).toInt();
			list.append(id);
		}
		m_lastError = "";
	}
	return list;
}

bool BaseObjectMapper::selectObject(Objects::BaseObject *object, quint32 id)
{
	QString text = QString("SELECT * FROM %1 WHERE Id = %2").arg(m_table).arg(id);
	QSqlQuery query;
	query.exec(text);
	if (query.next()) {
		object->m_id = query.value(0).toInt();
		setValuesFromFields(object, query);
		object->m_state = Objects::States::Selected;
		m_lastError = "";
		return true;
	}
	return false;
}

bool BaseObjectMapper::insertObject(Objects::BaseObject *object)
{
	QString text = QString("INSERT INTO %1 (%2) VALUES (%3)").arg(m_table).arg(m_columnList).arg(m_valueList);
	QSqlQuery query;
	query.prepare(text);
	setFieldsFromValues(object, query);
	query.exec();
	if (query.numRowsAffected() == 1) {
		object->m_id = query.lastInsertId().toInt();
		object->m_state = Objects::States::Selected;
		m_lastError = "";
		return true;
	}
	m_lastError = query.lastError().driverText();
	return false;
}

bool BaseObjectMapper::updateObject(Objects::BaseObject *object)
{
	QString text = QString("REPLACE INTO %1 (Id, %2) VALUES (:id, %3)").arg(m_table).arg(m_columnList).arg(m_valueList);
	QSqlQuery query;
	query.prepare(text);
	query.bindValue(":id", object->id());
	setFieldsFromValues(object, query);
	query.exec();
	if (query.numRowsAffected() == 1) {
		object->m_state = Objects::States::Selected;
		m_lastError = "";
		return true;
	}
	m_lastError = query.lastError().databaseText();
	return false;
}

bool BaseObjectMapper::deleteObject(Objects::BaseObject *object)
{
	QString text = QString("DELETE FROM %1 WHERE Id = %2").arg(m_table).arg(object->id());
	QSqlQuery query;
	if (query.exec(text)) {
		object->m_state = Objects::States::Deleted;
		m_lastError = "";
		return true;
	}
	m_lastError = query.lastError().databaseText();
	return false;
}

}
