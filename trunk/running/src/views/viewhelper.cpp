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

#include <QCompleter>
#include <QSqlQueryModel>

#include "viewhelper.h"

#include "../application.h"
#include "../objects/comboobject.h"
#include "../objects/shoe.h"
#include "../objects/shoemodel.h"
#include "../objects/shoemaker.h"

namespace ViewHelper
{

void fillComboBox(QComboBox *comboBox, Objects::Types::Type type, bool blankItem)
{
	int id = comboBox->itemData(comboBox->currentIndex()).toInt();

	QList<Objects::BaseObject *> list = Application::instance()->objectMap()->getAllObjects(type);

	comboBox->clear();

	if (blankItem) {
		comboBox->addItem("", 0);
	}

	if (type == Objects::Types::Shoe) {
		foreach (Objects::BaseObject *object, list) {
			Objects::Shoe *item = static_cast<Objects::Shoe *>(object);
			comboBox->addItem(
				QString("%1 %2").arg(item->shoeModel()->shoeMaker()->description())
					.arg(item->shoeModel()->description()),
				item->id());
		}
	} else {
		foreach (Objects::BaseObject *object, list) {
			Objects::ComboObject *item = static_cast<Objects::ComboObject *>(object);
			comboBox->addItem(item->description(), item->id());
		}
	}

	Application::instance()->objectMap()->discardObjects(list);

	if (id != -1) {
		if (comboBox->findData(id) != -1) {
			comboBox->setCurrentIndex(comboBox->findData(id));
		} else {
			comboBox->setCurrentIndex(0);
		}
	} else {
		comboBox->setCurrentIndex(0);
	}
}

Objects::BaseObject *getObjectOnComboBox(QComboBox *comboBox, Objects::Types::Type type, Objects::BaseObject *old_object)
{
	quint32 id = comboBox->itemData(comboBox->currentIndex()).toInt();

	if (old_object) {
		if (old_object->id() == id) {
			return old_object;
		} else {
			Application::instance()->objectMap()->discardObject(old_object);
		}
	}

	return Application::instance()->objectMap()->getObjectById(type, id);
}

void setIndexOnComboBox(QComboBox *comboBox, Objects::BaseObject *object)
{
	if (object) {
		comboBox->setCurrentIndex(comboBox->findData(object->id()));
	}
}

void setIndexOnComboBox(QComboBox *comboBox, int id)
{
	comboBox->setCurrentIndex(comboBox->findData(id));
}



QCompleter *completer(QWidget *widget, const QString &tableName, const QString &columnName)
{
	QCompleter *completer = new QCompleter(widget);

	QString query = QString("SELECT DISTINCT %1 FROM %2 ORDER BY %1").arg(columnName).arg(tableName);
	QSqlQueryModel *completerModel = new QSqlQueryModel(completer);
	completerModel->setQuery(query);

	completer->setModel(completerModel);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);

	return completer;
}

}
