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

#ifndef CHILDOBJECTTABLEMODEL_H
#define CHILDOBJECTTABLEMODEL_H

#include "../models/baseobjecttablemodel.h"

class ChildObjectTableModel : public BaseObjectTableModel
{
	Q_OBJECT

public:
	ChildObjectTableModel(Objects::Types::Type type, const QList<Objects::BaseObject *> &objects,
		 Objects::BaseObject *objectsParent, QObject *parent = 0);
	~ChildObjectTableModel();

	virtual bool submitAll();

protected:
	virtual void addToParent(Objects::BaseObject *object);
	virtual void removeFromParent(Objects::BaseObject *object);

	Objects::BaseObject *m_objectsParent;
};

#endif
