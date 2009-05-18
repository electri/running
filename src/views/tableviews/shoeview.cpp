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

#include <QtGui>
#include <QtSql>
#include "shoeview.h"
#include "settings.h"
#include "delegates/booleanimageitemdelegate.h"
#include "delegates/comboboxitemdelegate.h"
#include "utility/comboboxhelper.h"
#include "views/tableviews/shoemakerview.h"
#include "views/tableviews/shoemodelview.h"
//#include "../utility/statisticsservice.h"

ShoeView::ShoeView(QWidget *parent, quint32 id)
	: AbstractTableView(parent)
{
	setupUi(this);

	m_model->setTable("Shoe");
	m_model->select();

	m_model->setHeaderData(1, Qt::Horizontal, tr("Maker"), Qt::DisplayRole);
	m_model->setHeaderData(2, Qt::Horizontal, tr("Model"), Qt::DisplayRole);
	m_model->setHeaderData(3, Qt::Horizontal, tr("Size"), Qt::DisplayRole);
	m_model->setHeaderData(4, Qt::Horizontal, tr("Purchase Date"), Qt::DisplayRole);
	m_model->setHeaderData(7, Qt::Horizontal, tr("Retired"), Qt::DisplayRole);

	m_tableView = tableView;

	tableView->setModel(m_model);
	tableView->hideColumn(0);
	tableView->setColumnWidth(1, 100);
	tableView->setItemDelegateForColumn(1, new ComboBoxItemDelegate("ShoeMaker", tableView));
	tableView->setColumnWidth(2, 150);
	tableView->setItemDelegateForColumn(2, new ComboBoxItemDelegate("ShoeModel", tableView));
	tableView->setColumnWidth(3, 50);
	tableView->setColumnWidth(4, 80);
	tableView->hideColumn(5);
	tableView->hideColumn(6);
	tableView->setColumnWidth(7, 60);
	tableView->setItemDelegateForColumn(7, new BooleanImageItemDelegate(":/images/accept", tableView));
	tableView->hideColumn(8);

	connect(tableView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(currentRowChanged(const QModelIndex &, const QModelIndex &)));

	ComboBoxHelper::fillComboBox(shoeMakerComboBox, "ShoeMaker", false);

	refresh(id);
}

ShoeView::~ShoeView()
{
}

void ShoeView::showEvent(QShowEvent *event)
{
	Q_UNUSED(event);

	initialDistanceDoubleSpinBox->setSuffix(QString(" %1").arg(Settings::instance()->distanceUnit()));
	priceDoubleSpinBox->setSuffix(QString(" %1").arg(Settings::instance()->currencyUnit()));
	distanceDoubleSpinBox->setSuffix(QString(" %1").arg(Settings::instance()->distanceUnit()));
	pricePerDistanceDoubleSpinBox->setSuffix(QString(" %1/%2").arg(Settings::instance()->currencyUnit()).arg(Settings::instance()->distanceUnit()));
}

void ShoeView::currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(previous);

	shoeMakerComboBox->blockSignals(true);
	shoeModelComboBox->blockSignals(true);
	int shoeMakerId = current.sibling(current.row(), 1).data().toInt();
	ComboBoxHelper::setSelected(shoeMakerComboBox, shoeMakerId);
	ComboBoxHelper::fillComboBox(shoeModelComboBox, "ShoeModel", QString("ShoeMakerId = %1").arg(shoeMakerId), false);
	int shoeModelId = current.sibling(current.row(), 2).data().toInt();
	ComboBoxHelper::setSelected(shoeModelComboBox, shoeModelId);
	shoeMakerComboBox->blockSignals(false);
	shoeModelComboBox->blockSignals(false);

	sizeDoubleSpinBox->setValue(current.sibling(current.row(), 3).data().toDouble());
	purchaseDateDateEdit->setDate(current.sibling(current.row(), 4).data().toDate());
	priceDoubleSpinBox->setValue(current.sibling(current.row(), 5).data().toDouble());
	initialDistanceDoubleSpinBox->setValue(current.sibling(current.row(), 6).data().toDouble());
	retiredCheckBox->setChecked(current.sibling(current.row(), 7).data().toBool());
	notesPlainTextEdit->setPlainText(current.sibling(current.row(), 8).data().toString());

//	quint32 shoeId = current.sibling(current.row(), 0).data().toInt();
//	StatisticsResults::Shoes r = Utility::StatisticsService::instance()->shoe(shoeId);
//	m_distance = r.distance;
//	qreal distance = m_distance + initialDistanceDoubleSpinBox->value();
//	distanceDoubleSpinBox->setValue(distance);
//	pricePerDistanceDoubleSpinBox->setValue((distance > 0.0) ? (priceDoubleSpinBox->value() / distance) : 0.0);
	distanceDoubleSpinBox->setValue(0.0);
	pricePerDistanceDoubleSpinBox->setValue(0.0);
}

void ShoeView::on_shoeMakerComboBox_currentIndexChanged(int index)
{
	int value = shoeMakerComboBox->itemData(index).toInt();
	onIntChanged(1, value);

	ComboBoxHelper::fillComboBox(shoeModelComboBox, "ShoeModel", QString("ShoeMakerId = %1").arg(value), false);
}

void ShoeView::on_shoeModelComboBox_currentIndexChanged(int index)
{
	int value = shoeModelComboBox->itemData(index).toInt();
	onIntChanged(2, value);
}

void ShoeView::on_sizeDoubleSpinBox_valueChanged(double value)
{
	onDoubleChanged(3, value);
}

void ShoeView::on_purchaseDateDateEdit_dateChanged(const QDate &value)
{
	onDateChanged(4, value);
}

void ShoeView::on_priceDoubleSpinBox_valueChanged(double value)
{
	onDoubleChanged(5, value);

//	qreal distance = distanceDoubleSpinBox->value();
//	pricePerDistanceDoubleSpinBox->setValue((distance > 0.0) ? (value / distance) : 0.0);
}

void ShoeView::on_initialDistanceDoubleSpinBox_valueChanged(double value)
{
	onDoubleChanged(6, value);

//	distanceDoubleSpinBox->setValue(m_distance + value);
}

void ShoeView::on_retiredCheckBox_stateChanged(int state)
{
	onBoolChanged(7, state);
}

void ShoeView::on_notesPlainTextEdit_textChanged()
{
	onPlainTextChanged(8, notesPlainTextEdit);
}

void ShoeView::on_shoeMakerToolButton_clicked()
{
	int id = ComboBoxHelper::selected(shoeMakerComboBox);

	ShoeMakerView view(this, id);
	int result = view.exec();
	if (result == QDialog::Accepted) {
		ComboBoxHelper::fillComboBox(shoeMakerComboBox, "ShoeMaker", false);

		ComboBoxItemDelegate *delegate = qobject_cast<ComboBoxItemDelegate *>(tableView->itemDelegateForColumn(1));
		if (delegate) {
			delegate->refreshItems();
		}
	}
}

void ShoeView::on_shoeModelToolButton_clicked()
{
	int id = ComboBoxHelper::selected(shoeModelComboBox);

	ShoeModelView view(this, id);
	int result = view.exec();
	if (result == QDialog::Accepted) {
		ComboBoxHelper::fillComboBox(shoeModelComboBox, "ShoeModel", QString("ShoeMakerId = %1").arg(ComboBoxHelper::selected(shoeMakerComboBox)), false);

		ComboBoxItemDelegate *delegate = qobject_cast<ComboBoxItemDelegate *>(tableView->itemDelegateForColumn(2));
		if (delegate) {
			delegate->refreshItems();
		}
	}
}

void ShoeView::setControlsEnabled(bool enable)
{
	shoeMakerComboBox->setEnabled(enable);
	shoeModelComboBox->setEnabled(enable);
	sizeDoubleSpinBox->setEnabled(enable);
	purchaseDateDateEdit->setEnabled(enable);
	priceDoubleSpinBox->setEnabled(enable);
	initialDistanceDoubleSpinBox->setEnabled(enable);
	retiredCheckBox->setEnabled(enable);
	notesPlainTextEdit->setEnabled(enable);
}
