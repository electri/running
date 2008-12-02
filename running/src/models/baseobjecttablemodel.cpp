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

#include "baseobjecttablemodel.h"

#include "../application.h"
#include "../services/objectmap.h"
#include "../services/transaction.h"
#include "../services/memento.h"

BaseObjectTableModel::BaseObjectTableModel(Objects::Types::Type type, QObject *parent)
	: QAbstractTableModel(parent)
{
	m_type = type;

	m_objectMap = Application::instance()->objectMap();
}

BaseObjectTableModel::~BaseObjectTableModel()
{
}

int BaseObjectTableModel::rowCount(const QModelIndex &) const
{
	return m_list.count();
}

int BaseObjectTableModel::columnCount(const QModelIndex &) const
{
	return getColumnCount();
}

QVariant BaseObjectTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) return QVariant();

	if (index.row() >= m_list.size()) return QVariant();

	if ((role != Qt::DisplayRole) && (role != Qt::EditRole)) return QVariant();

	Objects::BaseObject *object = static_cast<Objects::BaseObject *>(m_list.at(index.row())->copy());
	return this->getColumnValue(object, index.column());
}

QVariant BaseObjectTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole) return QVariant();

	if (orientation == Qt::Horizontal) {
		return this->getColumnHeader(section);
	} else {
		return QString("%1").arg(section + 1);
	}
}

Qt::ItemFlags BaseObjectTableModel::flags(const QModelIndex &index) const
{
	if (!index.isValid()) return Qt::ItemIsEnabled;

	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool BaseObjectTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.isValid() && role == Qt::EditRole) {
		Objects::BaseObject *object = static_cast<Objects::BaseObject *>(m_list.at(index.row())->copy());
		this->setColumnValue(object, index.column(), value);

		if (int forcedColumn = this->forceColumnChange(index.column())) {
			emit dataChanged(index.sibling(index.row(), forcedColumn), index.sibling(index.row(), forcedColumn));
		}
		emit dataChanged(index, index);

		return true;
	}
	return false;
}

bool BaseObjectTableModel::insertRows(int position, int rows, const QModelIndex &)
{
	beginInsertRows(QModelIndex(), position, position+rows-1);

	for (int row = 0; row < rows; ++row) {
		Objects::BaseObject *object = m_objectMap->createObject(m_type);
		setDefaultValues(object);
		m_list.insert(position, new Services::Memento(object));
	}

	endInsertRows();

	return true;
}

bool BaseObjectTableModel::removeRows(int position, int rows, const QModelIndex &)
{
	beginRemoveRows(QModelIndex(), position, position+rows-1);

	for (int row = 0; row < rows; ++row) {
		m_removed.append(m_list.takeAt(position));
	}

	endRemoveRows();

	return true;
}

void BaseObjectTableModel::revertAll()
{
	m_list << m_removed;
	m_removed.clear();

	int i = m_list.size();
	while (i > 0) {
		Services::Memento *memento = m_list.at(--i);
		if (memento->original()->state() == Objects::States::Created) {
			m_objectMap->discardObject(memento->original());
			delete memento;
			m_list.removeAt(i);
		} else {
			memento->revert();
		}
	}

	this->reset();
}

bool BaseObjectTableModel::submitAll()
{
	foreach (Services::Memento *memento, m_list) {
		memento->submit();
	}
	m_removed.clear();

	return true;
}

QModelIndex BaseObjectTableModel::indexById(quint32 id) const
{
	QModelIndexList list = this->match(this->index(0, 0), Qt::DisplayRole, id, 1, Qt::MatchExactly | Qt::MatchWrap);
	if (!list.isEmpty()) {
		return list[0];
	}
	return QModelIndex();
}

QString BaseObjectTableModel::lastError() const
{
	return m_objectMap->lastError();
}



int BaseObjectTableModel::getColumnCount() const
{
	return 1;
}

QString BaseObjectTableModel::getColumnHeader(int column) const
{
	switch (column) {
		case 0:		return "Id";
	}
	return QString("Column %1").arg(column + 1);
}

QVariant BaseObjectTableModel::getColumnValue(Objects::BaseObject *object, int column) const
{
	QVariant value;
	if (object) {
		switch (column) {
			case 0:		value = object->id();	break;
		}
	}
	return value;
}

void BaseObjectTableModel::setColumnValue(Objects::BaseObject *object, int column, const QVariant &)
{
	if (object) {
		switch (column) {
			case 0:		break;
		}
	}
}



void BaseObjectTableModel::setDefaultValues(Objects::BaseObject *)
{
}

int BaseObjectTableModel::forceColumnChange(int)
{
	return 0;
}



Objects::BaseObject *BaseObjectTableModel::child(Objects::Types::Type type, quint32 id, Objects::BaseObject *old_child)
{
	if (old_child) {
		if (old_child->id() == id) {
			return old_child;
		} else {
			m_objectMap->discardObject(old_child);
		}
	}

	return m_objectMap->getObjectById(type, id);
}
