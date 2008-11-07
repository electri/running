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

#ifndef BASEOBJECTTABLEMODEL_H
#define BASEOBJECTTABLEMODEL_H

#include <QAbstractTableModel>

#include "../objects/baseobject.h"
#include "../services/memento.h"

class BaseObjectTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	BaseObjectTableModel(Objects::Types::Type type, QObject *parent = 0);
	BaseObjectTableModel(Objects::Types::Type type, const QList<Objects::BaseObject *> &objects,
		 Objects::BaseObject *objectsParent, QObject *parent = 0);
	~BaseObjectTableModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
	bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

	void revertAll();
	bool submitAll();
	
	QModelIndex indexById(quint32 id) const;
	QString lastError() const;

protected:
	virtual int getColumnCount() const;
	virtual QString getColumnHeader(int column) const;
	virtual QVariant getColumnValue(Objects::BaseObject *object, int column) const;
	virtual void setColumnValue(Objects::BaseObject *object, int column, const QVariant &value);

	virtual int forceColumnChange(int column);

	Objects::BaseObject *child(Objects::Types::Type type, quint32 id, Objects::BaseObject *old_child);

	QList<Services::Memento *> m_list;
	QList<Services::Memento *> m_removed;
	Objects::Types::Type m_type;
	QList<Objects::BaseObject *> m_objects;
	Objects::BaseObject *m_objectsParent;
};

#endif
