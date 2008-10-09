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

#include "../services/objectmap.h"
#include "../services/objectrepository.h"

BaseObjectTableModel::BaseObjectTableModel(Objects::Types::Type type, QObject *parent)
	: QAbstractTableModel(parent)
{
	m_type = type;
	m_parent = NULL;

	Services::ObjectMap *session = Services::ObjectMap::instance();

	QList<Objects::BaseObject *> objects = session->getAllObjects(m_type);
	foreach (Objects::BaseObject *object, objects) {
		m_list.append(new Services::Memento(object));
	}
}

BaseObjectTableModel::BaseObjectTableModel(Objects::Types::Type type, Objects::BaseObject *parentObject, QObject *parent)
	: QAbstractTableModel(parent)
{
	m_type = type;
	m_parent = parentObject;

	Services::ObjectMap *session = Services::ObjectMap::instance();

	QList<Objects::BaseObject *> objects = session->getObjectsByParent(m_type, m_parent);
	foreach (Objects::BaseObject *object, objects) {
		m_list.append(new Services::Memento(object));
	}
}

BaseObjectTableModel::~BaseObjectTableModel()
{
	Services::ObjectMap *session = Services::ObjectMap::instance();

	foreach (Services::Memento *memento, m_list) {
		session->discardObject(memento->original());
		delete memento;
	}
	foreach (Services::Memento *memento, m_removed) {
		session->discardObject(memento->original());
		delete memento;
	}
}

int BaseObjectTableModel::rowCount(const QModelIndex &parent) const
{
	return m_list.count();
}

int BaseObjectTableModel::columnCount(const QModelIndex &parent) const
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

bool BaseObjectTableModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	beginInsertRows(QModelIndex(), position, position+rows-1);

	Services::ObjectMap *session = Services::ObjectMap::instance();

	for (int row = 0; row < rows; ++row) {
		Objects::BaseObject *object = session->createObject(m_type);
		if (m_parent) {
			object->setParent(m_parent);
		}
		m_list.insert(position, new Services::Memento(object));
	}

	endInsertRows();
	return true;
}

bool BaseObjectTableModel::removeRows(int position, int rows, const QModelIndex &parent)
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
	Services::ObjectMap *session = Services::ObjectMap::instance();

	foreach (Services::Memento *memento, m_removed) {
		m_list.append(memento);
	}
	m_removed.clear();

	int i = m_list.size();
	while (i > 0) {
		Services::Memento *memento = m_list.at(--i);
		if (memento->original()->state() == Objects::States::Created) {
			session->discardObject(memento->original());
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
	Services::ObjectMap *session = Services::ObjectMap::instance();
	Services::ObjectRepository *repository = Services::ObjectRepository::instance();
	if (repository->transaction()) {
		foreach (Services::Memento *memento, m_list) {
			if (!session->saveObject(memento->copy())) {
				repository->rollback();
				return false;
			}
		}
		foreach (Services::Memento *memento, m_removed) {
			if (!session->deleteObject(memento->copy())) {
				repository->rollback();
				return false;
			}
		}
		repository->commit();
	} else {
		return false;
	}
	foreach (Services::Memento *memento, m_list) {
		memento->submit();
	}
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

void BaseObjectTableModel::setColumnValue(Objects::BaseObject *object, int column, const QVariant &value)
{
	if (object) {
		switch (column) {
			case 0:		break;
		}
	}
}

int BaseObjectTableModel::forceColumnChange(int /*column*/)
{
	return 0;
}
