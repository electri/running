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

#ifndef INTERVALTABLEMODEL_H
#define INTERVALTABLEMODEL_H

#include "../models/childobjecttablemodel.h"

namespace Objects { class Event; }

class IntervalTableModel : public ChildObjectTableModel
{
	Q_OBJECT

public:
	IntervalTableModel(Objects::Event *event, QObject *parent = 0);
	~IntervalTableModel();

private:
	int getColumnCount() const;
	QString getColumnHeader(int column) const;
	QVariant getColumnValue(Objects::BaseObject *object, int column) const;
	void setColumnValue(Objects::BaseObject *object, int column, const QVariant &value);

	void setDefaultValues(Objects::BaseObject *object);

	void addToParent(Objects::BaseObject *object);
	void removeFromParent(Objects::BaseObject *object);
};

#endif
