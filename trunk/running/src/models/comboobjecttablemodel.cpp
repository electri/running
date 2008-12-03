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

#include "comboobjecttablemodel.h"
#include "../services/objectmap.h"
#include "../services/memento.h"

ComboObjectTableModel::ComboObjectTableModel(Objects::Types::Type type, QObject *parent)
	: BaseObjectTableModel(type, parent)
{
	QList<Objects::BaseObject *> objects = m_objectMap->getAllObjects(m_type);

	foreach (Objects::BaseObject *object, objects) {
		m_list.append(new Services::Memento(object));
	}
}

ComboObjectTableModel::~ComboObjectTableModel()
{
	foreach (Services::Memento *memento, m_list) {
		m_objectMap->discardObject(memento->original());
		delete memento;
	}
	foreach (Services::Memento *memento, m_removed) {
		m_objectMap->discardObject(memento->original());
		delete memento;
	}
}



bool ComboObjectTableModel::submitAll()
{
	QList<Objects::BaseObject *> list;
	foreach (Services::Memento *memento, m_list) {
		list << memento->copy();
	}
	QList<Objects::BaseObject *> removed;
	foreach (Services::Memento *memento, m_removed) {
		removed << memento->copy();
	}
	if (!m_objectMap->updateObjects(list, removed)) {
		return false;
	}

	foreach (Services::Memento *memento, m_list) {
		memento->submit();
	}
	foreach (Services::Memento *memento, m_removed) {
		m_objectMap->discardObject(memento->original());
		delete memento;
	}
	m_removed.clear();

	return true;
}



int ComboObjectTableModel::getColumnCount() const
{
	return 2;
}

QString ComboObjectTableModel::getColumnHeader(int column) const
{
	switch (column) {
		case 0:	return "Id";
		case 1:	return tr("Description");
	}
	return QString("Column %1").arg(column + 1);
}

QVariant ComboObjectTableModel::getColumnValue(Objects::BaseObject *object, int column) const
{
	QVariant value;
	Objects::ComboObject *comboObject = static_cast<Objects::ComboObject *>(object);
	if (comboObject) {
		switch (column) {
			case 0:	value = comboObject->id();				break;
			case 1:	value = comboObject->description();		break;
		}
	}
	return value;
}

void ComboObjectTableModel::setColumnValue(Objects::BaseObject *object, int column, const QVariant &value)
{
	Objects::ComboObject *comboObject = static_cast<Objects::ComboObject *>(object);
	if (comboObject) {
		switch (column) {
			case 0:	break;
			case 1:	comboObject->setDescription(value.toString());	break;
		}
	}
}
