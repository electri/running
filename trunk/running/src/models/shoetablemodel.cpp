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

#include "shoetablemodel.h"

#include "../objects/shoe.h"
#include "../objects/shoemaker.h"
#include "../objects/shoemodel.h"
#include "../services/objectmap.h"
#include "../services/memento.h"

ShoeTableModel::ShoeTableModel(QObject *parent)
	: BaseObjectTableModel(Objects::Types::Shoe, parent)
{
	QList<Objects::BaseObject *> objects = m_objectMap->getAllObjects(Objects::Types::Shoe);

	foreach (Objects::BaseObject *object, objects) {
		m_list.append(new Services::Memento(object));
	}
}

ShoeTableModel::~ShoeTableModel()
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



bool ShoeTableModel::submitAll()
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
	m_removed.clear();

	return true;
}



int ShoeTableModel::getColumnCount() const
{
	return 9;
}

QString ShoeTableModel::getColumnHeader(int column) const
{
	switch (column) {
		case 0:		return "Id";
		case 1:		return tr("Maker");
		case 2:		return tr("Model");
		case 3:		return tr("Size");
		case 4:		return tr("Purchase");
		case 5:		return tr("Price");
		case 6:		return tr("Initial distance");
		case 7:		return tr("Retired");
		case 8:		return tr("Notes");
	}
	return QString("Column %1").arg(column + 1);
}

QVariant ShoeTableModel::getColumnValue(Objects::BaseObject *object, int column) const
{
	QVariant value;
	Objects::Shoe *shoe = static_cast<Objects::Shoe *>(object);
	if (shoe) {
		switch (column) {
			case 0:		value = shoe->id();					break;
			case 1:		value = shoe->shoeModel() ? shoe->shoeModel()->shoeMaker() ? shoe->shoeModel()->shoeMaker()->id() : 0 : 0;		break;
			case 2:		value = shoe->shoeModel() ? shoe->shoeModel()->id() : 0;		break;
			case 3:		value = shoe->size();				break;
			case 4:		value = shoe->purchaseDate();		break;
			case 5:		value = shoe->price();				break;
			case 6:		value = shoe->initialDistance();	break;
			case 7:		value = shoe->retired();			break;
			case 8:		value = shoe->notes();				break;
		}
	}
	return value;
}

void ShoeTableModel::setColumnValue(Objects::BaseObject *object, int column, const QVariant &value)
{
	Objects::Shoe *shoe = static_cast<Objects::Shoe *>(object);
	if (shoe) {
		switch (column) {
			case 0:		break;
			case 1:		break;
			case 2:		shoe->setShoeModel(static_cast<Objects::ShoeModel *>(this->child(Objects::Types::ShoeModel, value.toInt(), shoe->shoeModel())));	break;
			case 3:		shoe->setSize(value.toDouble());			break;
			case 4:		shoe->setPurchaseDate(value.toDate());		break;
			case 5:		shoe->setPrice(value.toDouble());			break;
			case 6:		shoe->setInitialDistance(value.toDouble());	break;
			case 7:		shoe->setRetired(value.toBool());			break;
			case 8:		shoe->setNotes(value.toString());			break;
		}
	}
}



int ShoeTableModel::forceColumnChange(int column)
{
	switch (column) {
		case 2:		return 1;
	}
	return 0;
}
