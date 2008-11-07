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

#include "shoemodeltablemodel.h"

#include "../objects/shoemodel.h"
#include "../objects/shoemaker.h"

ShoeModelTableModel::ShoeModelTableModel(QObject *parent)
	: ComboObjectTableModel(Objects::Types::ShoeModel, parent)
{
}

ShoeModelTableModel::~ShoeModelTableModel()
{
}



int ShoeModelTableModel::getColumnCount() const
{
	return 3;
}

QString ShoeModelTableModel::getColumnHeader(int column) const
{
	switch (column) {
		case 0:		return "Id";
		case 1:		return tr("Maker");
		case 2:		return tr("Description");
	}
	return QString("Column %1").arg(column + 1);
}

QVariant ShoeModelTableModel::getColumnValue(Objects::BaseObject *object, int column) const
{
	QVariant value;
	Objects::ShoeModel *shoeModel = static_cast<Objects::ShoeModel *>(object);
	if (shoeModel) {
		switch (column) {
			case 0:		value = shoeModel->id();			break;
			case 1:		value = shoeModel->shoeMaker() ? shoeModel->shoeMaker()->id() : 0;		break;
			case 2:		value = shoeModel->description();	break;
		}
	}
	return value;
}

void ShoeModelTableModel::setColumnValue(Objects::BaseObject *object, int column, const QVariant &value)
{
	Objects::ShoeModel *shoeModel = static_cast<Objects::ShoeModel *>(object);
	if (shoeModel) {
		switch (column) {
			case 0:		break;
			case 1:		shoeModel->setShoeMaker(static_cast<Objects::ShoeMaker *>(
							this->child(Objects::Types::ShoeMaker, value.toInt(), shoeModel->shoeMaker())));	break;
			case 2:		shoeModel->setDescription(value.toString());	break;
		}
	}
}
