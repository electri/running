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

#include "eventtypetablemodel.h"

#include "../objects/eventtype.h"

EventTypeTableModel::EventTypeTableModel(QObject *parent)
	: ComboObjectTableModel(Objects::Types::EventType, parent)
{
}

EventTypeTableModel::~EventTypeTableModel()
{
}



int EventTypeTableModel::getColumnCount() const
{
	return 4;
}

QString EventTypeTableModel::getColumnHeader(int column) const
{
	switch (column) {
		case 0:		return "Id";
		case 1:		return tr("Description");
		case 2:		return tr("Medal");
		case 3:		return tr("Intervals");
	}
	return QString("Column %1").arg(column + 1);
}

QVariant EventTypeTableModel::getColumnValue(Objects::BaseObject *object, int column) const
{
	QVariant value;
	Objects::EventType *eventType = static_cast<Objects::EventType *>(object);
	if (eventType) {
		switch (column)
		{
			case 0:		value = eventType->id();			break;
			case 1:		value = eventType->description();	break;
			case 2:		value = eventType->hasMedal();		break;
			case 3:		value = eventType->hasIntervals();	break;
		}
	}
	return value;
}

void EventTypeTableModel::setColumnValue(Objects::BaseObject *object, int column, const QVariant &value)
{
	Objects::EventType *eventType = static_cast<Objects::EventType *>(object);
	if (eventType) {
		switch (column)
		{
			case 0:		break;
			case 1:		eventType->setDescription(value.toString());	break;
			case 2:		eventType->setMedal(value.toBool());			break;
			case 3:		eventType->setIntervals(value.toBool());		break;
		}
	}
}
