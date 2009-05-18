/****************************************************************************

	running - A small program to keep track of your workouts.
	Copyright (C) 2009  Marco Gasparetto (markgabbahey@gmail.com)

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

#include <QSqlQuery>
#include "comboboxhelper.h"

void ComboBoxHelper::fillComboBox(QComboBox *comboBox, const QString &tableName, bool blankItem)
{
	fillComboBox(comboBox, tableName, "", blankItem);
}

void ComboBoxHelper::fillComboBox(QComboBox *comboBox, const QString &tableName, const QString &filter, bool blankItem)
{
	int id = comboBox->itemData(comboBox->currentIndex()).toInt();

	comboBox->clear();
	if (blankItem) {
		comboBox->addItem("", 0);
	}

	QString queryText = QString("SELECT Id, Description FROM %1").arg(tableName);
	if (!filter.isEmpty()) {
		queryText += QString(" WHERE %1").arg(filter);
	}
	queryText += QString(" ORDER BY Description");

	QSqlDatabase db = QSqlDatabase::database();
	if (db.isValid()) {
		if (db.isOpen()) {
			QSqlQuery query(db);
			query.exec(queryText);
			while (query.next()) {
				int id = query.value(0).toInt();
				QString description = query.value(1).toString();
				comboBox->addItem(description, id);
			}
		}
	}

	if (id != -1) {
		if (comboBox->findData(id) != -1) {
			comboBox->setCurrentIndex(comboBox->findData(id));
			return;
		}
	}
	comboBox->setCurrentIndex(0);
}

void ComboBoxHelper::fillShoesComboBox(QComboBox *comboBox, bool blankItem)
{
	int id = comboBox->itemData(comboBox->currentIndex()).toInt();

	comboBox->clear();
	if (blankItem) {
		comboBox->addItem("", 0);
	}

	QString queryText = QString("SELECT Shoe.Id, ShoeMaker.Description || ' ' || ShoeModel.Description AS Description FROM Shoe LEFT JOIN ShoeModel ON ShoeModel.Id = Shoe.ShoeModelId LEFT JOIN ShoeMaker ON ShoeMaker.Id = ShoeModel.ShoeMakerId");
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isValid()) {
		if (db.isOpen()) {
			QSqlQuery query(db);
			query.exec(queryText);
			while (query.next()) {
				int id = query.value(0).toInt();
				QString description = query.value(1).toString();
				comboBox->addItem(description, id);
			}
		}
	}

	if (id != -1) {
		if (comboBox->findData(id) != -1) {
			comboBox->setCurrentIndex(comboBox->findData(id));
			return;
		}
	}
	comboBox->setCurrentIndex(0);
}

//void fillShoeMakerModelsComboBox(QComboBox *comboBox)
//{
//	int id = comboBox->itemData(comboBox->currentIndex()).toInt();
//
//	comboBox->clear();
//
//	QList<Objects::BaseObject *> list = Application::instance()->objectMap()->getAllObjects(Objects::Types::ShoeModel);
//	foreach (Objects::BaseObject *object, list) {
//		Objects::ShoeModel *item = static_cast<Objects::ShoeModel *>(object);
//		comboBox->addItem(QString("%1 %2").arg(item->shoeMaker()->description())
//				.arg(item->description()), item->id());
//	}
//	Application::instance()->objectMap()->discardObjects(list);
//
//	if (id != -1) {
//		if (comboBox->findData(id) != -1) {
//			comboBox->setCurrentIndex(comboBox->findData(id));
//			return;
//		}
//	}
//	comboBox->setCurrentIndex(0);
//}

void ComboBoxHelper::setSelected(QComboBox *comboBox, int id)
{
	comboBox->setCurrentIndex(comboBox->findData(id));
}

void ComboBoxHelper::setSelected(QComboBox *comboBox, const QString &text)
{
	comboBox->setCurrentIndex(comboBox->findText(text));
}

int ComboBoxHelper::selected(QComboBox *comboBox)
{
	return comboBox->itemData(comboBox->currentIndex()).toInt();
}
