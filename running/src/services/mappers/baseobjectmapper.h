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

#ifndef BASEOBJECTMAPPER_H
#define BASEOBJECTMAPPER_H

#include <QList>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "../../objects/baseobject.h"

namespace Mappers {

class BaseObjectMapper
{
public:
	BaseObjectMapper();
	virtual ~BaseObjectMapper();

	virtual QList<quint32> selectIdList();
	virtual QList<quint32> selectIdList(Objects::BaseObject *parent);
	virtual bool selectObject(Objects::BaseObject *, quint32 id);
	virtual bool insertObject(Objects::BaseObject *);
	virtual bool updateObject(Objects::BaseObject *);
	virtual bool deleteObject(Objects::BaseObject *);

	QString lastError() const { return m_lastError; }

protected:
	virtual void setValuesFromFields(Objects::BaseObject *, QSqlQuery &) = 0;
	virtual void setFieldsFromValues(Objects::BaseObject *, QSqlQuery &) = 0;

	Objects::BaseObject *child(quint32 id, Objects::Types::Type type, Objects::BaseObject *old_child);

	QSqlDatabase m_database;
	QString m_table;
	QString m_order;
	QString m_columnList, m_valueList;
	QString m_parentColumn;
	QString m_lastError;
};

}

#endif
