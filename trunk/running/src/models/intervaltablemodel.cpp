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

#include "intervaltablemodel.h"

#include "../objects/interval.h"
#include "../objects/intervaltype.h"
#include "../objects/event.h"

IntervalTableModel::IntervalTableModel(Objects::Event *event, QObject *parent)
	: BaseObjectTableModel(Objects::Types::Interval, event->intervals(), event, parent)
{
}

IntervalTableModel::~IntervalTableModel()
{
}



int IntervalTableModel::getColumnCount() const
{
	return 5;
}

QString IntervalTableModel::getColumnHeader(int column) const
{
	switch (column) {
		case 0:		return "Id";
		case 1:		return tr("Interval type");
		case 2:		return tr("Distance");
		case 3:		return tr("Duration");
		case 4:		return tr("Notes");
	}
	return QString("Column %1").arg(column + 1);
}

QVariant IntervalTableModel::getColumnValue(Objects::BaseObject *object, int column) const
{
	QVariant value;
	Objects::Interval *interval = static_cast<Objects::Interval *>(object);
	if (interval) {
		switch (column) {
			case 0:		value = interval->id();				break;
			case 1:		value = interval->intervalType() ? interval->intervalType()->id() : 0;		break;
			case 2:		value = interval->distance();		break;
			case 3:		value = interval->duration();		break;
			case 4:		value = interval->notes();			break;
		}
	}
	return value;
}

void IntervalTableModel::setColumnValue(Objects::BaseObject *object, int column, const QVariant &value)
{
	Objects::Interval *interval = static_cast<Objects::Interval *>(object);
	if (interval) {
		switch (column) {
			case 0:		break;
			case 1:		interval->setIntervalType(static_cast<Objects::IntervalType *>(
							this->child(Objects::Types::IntervalType, value.toInt(), interval->intervalType())));	break;
			case 2:		interval->setDistance(value.toDouble());	break;
			case 3:		interval->setDuration(value.toTime());		break;
			case 4:		interval->setNotes(value.toString());		break;
		}
	}
}
