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

#include <QtGui>

#include "shoeview.h"

#include "../objects/shoe.h"
#include "../objects/shoemaker.h"
#include "../objects/shoemodel.h"
#include "../delegates/booleanimageitemdelegate.h"
#include "../delegates/comboobjectitemdelegate.h"
#include "../models/shoetablemodel.h"
#include "../services/objectmap.h"
#include "../services/objectrepository.h"
#include "../views/shoemakerview.h"
#include "../views/shoemodelview.h"
#include "../utility/statisticsservice.h"

ShoeView::ShoeView(QWidget *parent, quint32 id)
	: QDialog(parent)
{
	setupUi(this);

	m_model = new ShoeTableModel(this);

	tableView->setModel(m_model);
	tableView->hideColumn(0);
	tableView->setColumnWidth(1, 70);
	tableView->setItemDelegateForColumn(1, new ComboObjectItemDelegate(Objects::Types::ShoeMaker, tableView));
	tableView->setColumnWidth(2, 100);
	tableView->setItemDelegateForColumn(2, new ComboObjectItemDelegate(Objects::Types::ShoeModel, tableView));
	tableView->setColumnWidth(3, 50);
	tableView->setColumnWidth(4, 80);
	tableView->hideColumn(5);
	tableView->hideColumn(6);
	tableView->setColumnWidth(7, 30);
	tableView->setItemDelegateForColumn(7, new BooleanImageItemDelegate(":yes", tableView));
	tableView->hideColumn(8);

	connect(tableView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
									 this, SLOT(currentRowChanged(const QModelIndex &, const QModelIndex &)));

	refreshComboBoxes();

	if (m_model->rowCount() > 0) {
		if (id) {
			tableView->setCurrentIndex(m_model->index(m_model->indexById(id).row(), 1));
		} else {
			tableView->setCurrentIndex(m_model->index(0, 1));
		}
	} else {
		this->setControlsEnabled(false);
	}
}

ShoeView::~ShoeView()
{
	delete m_model;
}



void ShoeView::on_addPushButton_clicked()
{
	if (m_model->rowCount() > 0) {
		int row = tableView->currentIndex().row() + 1;
		m_model->insertRows(row, 1);
		tableView->setCurrentIndex(m_model->index(row, 1));
	} else {
		m_model->insertRows(0, 1);
		tableView->setCurrentIndex(m_model->index(0, 1));
		this->setControlsEnabled(true);
	}
}

void ShoeView::on_removePushButton_clicked()
{
	if (m_model->rowCount() > 0) {
		QModelIndexList indexes = tableView->selectionModel()->selectedIndexes();
		if (!indexes.isEmpty()) {
			m_model->removeRows(indexes.at(0).row(), 1);
		}
		if (m_model->rowCount() == 0) {
			this->setControlsEnabled(false);
		}
	}
}

void ShoeView::on_resetPushButton_clicked()
{
	m_model->revertAll();
	if (m_model->rowCount() > 0) {
		tableView->setCurrentIndex(m_model->index(0, 1));
		this->setControlsEnabled(true);
	} else {
		this->setControlsEnabled(false);
	}
}

void ShoeView::on_savePushButton_clicked()
{
	bool result = m_model->submitAll();
	if (!result) {
		QMessageBox::critical(this, tr("Error"),
				tr("An error has occoured during saving modifications in the database.") +
				"\n\n" + Services::ObjectRepository::instance()->lastError());
		return;
	}
	this->accept();
}

void ShoeView::on_cancelPushButton_clicked()
{
	this->reject();
}



void ShoeView::currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
	shoeMakerComboBox->setCurrentIndex(shoeMakerComboBox->findData(current.sibling(current.row(), 1).data().toInt()));
	shoeModelComboBox->setCurrentIndex(shoeModelComboBox->findData(current.sibling(current.row(), 2).data().toInt()));
	sizeDoubleSpinBox->setValue(current.sibling(current.row(), 3).data().toDouble());
	purchaseDateDateEdit->setDate(current.sibling(current.row(), 4).data().toDate());
	priceDoubleSpinBox->setValue(current.sibling(current.row(), 5).data().toDouble());
	initialDistanceDoubleSpinBox->setValue(current.sibling(current.row(), 6).data().toDouble());
	retiredCheckBox->setChecked(current.sibling(current.row(), 7).data().toBool());
	notesPlainTextEdit->setPlainText(current.sibling(current.row(), 8).data().toString());

	quint32 shoeId = current.sibling(current.row(), 0).data().toInt();
	StatisticsResults::Shoes r = Utility::StatisticsService::instance()->shoe(shoeId);
	m_distance = r.distance;
	qreal distance = m_distance + initialDistanceDoubleSpinBox->value();
	distanceDoubleSpinBox->setValue(distance);
	pricePerDistanceDoubleSpinBox->setValue((distance > 0.0) ? (priceDoubleSpinBox->value() / distance) : 0.0);
}



void ShoeView::on_shoeMakerComboBox_currentIndexChanged(int index)
{
	quint32 shoeMakerId = shoeMakerComboBox->itemData(index).toInt();

	Services::ObjectMap *session = Services::ObjectMap::instance();

	shoeModelComboBox->clear();
	QList<Objects::BaseObject *> list = session->getAllObjects(Objects::Types::ShoeModel);
	QList<Objects::BaseObject *>::const_iterator it = list.constBegin();
	while (it != list.constEnd()) {
		Objects::ShoeModel *item = static_cast<Objects::ShoeModel *>(*it++);
		if (item->shoeMaker()->id() == shoeMakerId) {
			shoeModelComboBox->addItem(item->description(), item->id());
		}
	}
	session->discardObjects(list);

	quint32 shoeModelId = shoeModelComboBox->itemData(shoeModelComboBox->currentIndex()).toInt();
	shoeModelComboBox->setCurrentIndex(shoeModelComboBox->findData(shoeModelId));
}

void ShoeView::on_shoeModelComboBox_currentIndexChanged(int index)
{
	quint32 shoeModelId = shoeModelComboBox->itemData(index).toInt();
	tableView->model()->setData(tableView->currentIndex().sibling(tableView->currentIndex().row(), 2), shoeModelId);
}

void ShoeView::on_sizeDoubleSpinBox_valueChanged(double value)
{
	tableView->model()->setData(tableView->currentIndex().sibling(tableView->currentIndex().row(), 3), value);
}

void ShoeView::on_purchaseDateDateEdit_dateChanged(const QDate &value)
{
	tableView->model()->setData(tableView->currentIndex().sibling(tableView->currentIndex().row(), 4), value);
}

void ShoeView::on_priceDoubleSpinBox_valueChanged(double value)
{
	tableView->model()->setData(tableView->currentIndex().sibling(tableView->currentIndex().row(), 5), value);

	qreal distance = distanceDoubleSpinBox->value();
	pricePerDistanceDoubleSpinBox->setValue((distance > 0.0) ? (value / distance) : 0.0);
}

void ShoeView::on_initialDistanceDoubleSpinBox_valueChanged(double value)
{
	tableView->model()->setData(tableView->currentIndex().sibling(tableView->currentIndex().row(), 6), value);

	distanceDoubleSpinBox->setValue(m_distance + value);
}

void ShoeView::on_retiredCheckBox_stateChanged(int state)
{
	tableView->model()->setData(tableView->currentIndex().sibling(tableView->currentIndex().row(), 7), state > 0);
}

void ShoeView::on_notesPlainTextEdit_textChanged()
{
	tableView->model()->setData(tableView->currentIndex().sibling(tableView->currentIndex().row(), 8), notesPlainTextEdit->toPlainText());
}



void ShoeView::on_shoeMakerToolButton_clicked()
{
	ShoeMakerView *view = new ShoeMakerView(this);
	int result = view->exec();
	if (result == QDialog::Accepted) {
		quint32 id = shoeModelComboBox->itemData(shoeModelComboBox->currentIndex()).toInt();
		this->refreshComboBoxes();
		shoeModelComboBox->setCurrentIndex(shoeModelComboBox->findData(id));
	}
	delete view;
}

void ShoeView::on_shoeModelToolButton_clicked()
{
	ShoeModelView *view = new ShoeModelView(this);
	int result = view->exec();
	if (result == QDialog::Accepted) {
		quint32 id = shoeModelComboBox->itemData(shoeModelComboBox->currentIndex()).toInt();
		this->refreshComboBoxes();
		shoeModelComboBox->setCurrentIndex(shoeModelComboBox->findData(id));
	}
	delete view;
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

	if (!enable) {
		distanceDoubleSpinBox->clear();
		pricePerDistanceDoubleSpinBox->clear();
	}
}

void ShoeView::refreshComboBoxes()
{
	Services::ObjectMap *session = Services::ObjectMap::instance();

	quint32 id = shoeMakerComboBox->itemData(shoeMakerComboBox->currentIndex()).toInt();
	shoeMakerComboBox->clear();
	QList<Objects::BaseObject *> list = session->getAllObjects(Objects::Types::ShoeMaker);
	foreach (Objects::BaseObject *object, list) {
		Objects::ComboObject *item = static_cast<Objects::ComboObject *>(object);
		shoeMakerComboBox->addItem(item->description(), item->id());
	}
	session->discardObjects(list);
	shoeMakerComboBox->setCurrentIndex(shoeMakerComboBox->findData(id));
}
