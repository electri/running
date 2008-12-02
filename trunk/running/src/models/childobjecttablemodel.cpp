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

#include "childobjecttablemodel.h"
#include "../services/objectmap.h"
#include "../services/memento.h"

ChildObjectTableModel::ChildObjectTableModel(Objects::Types::Type type, const QList<Objects::BaseObject *> &objects, Objects::BaseObject *objectsParent, QObject *parent)
	: BaseObjectTableModel(type, parent)
{
	m_objects = objects;
	m_objectsParent = objectsParent;

	foreach (Objects::BaseObject *object, objects) {
		m_list.append(new Services::Memento(object));
	}
}

ChildObjectTableModel::~ChildObjectTableModel()
{
	// TODO

//	foreach (Services::Memento *memento, m_list) {
//		m_objectMap->discardObject(memento->original());
//		delete memento;
//	}
//	foreach (Services::Memento *memento, m_removed) {
//		m_objectMap->discardObject(memento->original());
//		delete memento;
//	}
}



bool ChildObjectTableModel::submitAll()
{
	foreach (Services::Memento *memento, m_list) {
		memento->submit();
	}

	if (m_objectsParent) {
		foreach (Services::Memento *memento, m_list) {
			if (m_objects.contains(memento->original()) == false) {
				this->addToParent(memento->original());
			}
		}
		foreach (Services::Memento *memento, m_removed) {
			if (m_objects.contains(memento->original()) == true) {
				this->removeFromParent(memento->original());
			}
		}
	}

	return true;
}



void ChildObjectTableModel::addToParent(Objects::BaseObject *object)
{
}

void ChildObjectTableModel::removeFromParent(Objects::BaseObject *object)
{
}
