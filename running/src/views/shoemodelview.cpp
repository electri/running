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

#include "shoemodelview.h"

#include "../delegates/comboobjectitemdelegate.h"
#include "../models/shoemodeltablemodel.h"
#include "../views/viewhelper.h"

ShoeModelView::ShoeModelView(QWidget *parent, quint32 id)
	: QDialog(parent)
{
	setupUi(this);

	m_model = new ShoeModelTableModel(this);

	tableView->setModel(m_model);
	tableView->hideColumn(0);
	tableView->setColumnWidth(1, 130);
	tableView->setItemDelegateForColumn(1, new ComboObjectItemDelegate(Objects::Types::ShoeMaker, tableView));
	tableView->setColumnWidth(2, 200);

	connect(tableView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
									 this, SLOT(currentRowChanged(const QModelIndex &, const QModelIndex &)));

	descriptionLineEdit->setCompleter(ViewHelper::completer(descriptionLineEdit, "ShoeModel", "Description"));

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

ShoeModelView::~ShoeModelView()
{
	delete m_model;
}



void ShoeModelView::on_addPushButton_clicked()
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

void ShoeModelView::on_removePushButton_clicked()
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

void ShoeModelView::on_resetPushButton_clicked()
{
	m_model->revertAll();
	if (m_model->rowCount() > 0) {
		tableView->setCurrentIndex(m_model->index(0, 1));
		this->setControlsEnabled(true);
	} else {
		this->setControlsEnabled(false);
	}
}

void ShoeModelView::on_savePushButton_clicked()
{
	bool result = m_model->submitAll();
	if (!result) {
		QMessageBox::critical(this, tr("Error"), m_model->lastError());
		return;
	}
	this->accept();
}

void ShoeModelView::on_cancelPushButton_clicked()
{
	this->reject();
}



void ShoeModelView::currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(previous);

	shoeMakerComboBox->setCurrentIndex(shoeMakerComboBox->findData(current.sibling(current.row(), 1).data().toInt()));
	descriptionLineEdit->setText(current.sibling(current.row(), 2).data().toString());
}



void ShoeModelView::on_descriptionLineEdit_textChanged(const QString &text)
{
	tableView->model()->setData(tableView->currentIndex().sibling(tableView->currentIndex().row(), 2), text);
}

void ShoeModelView::on_shoeMakerComboBox_currentIndexChanged(int state)
{
	Q_UNUSED(state);

	quint32 shoeMakerId = shoeMakerComboBox->itemData(shoeMakerComboBox->currentIndex()).toInt();
	tableView->model()->setData(tableView->currentIndex().sibling(tableView->currentIndex().row(), 1), shoeMakerId);
}



void ShoeModelView::setControlsEnabled(bool enable)
{
	shoeMakerComboBox->setEnabled(enable);
	descriptionLineEdit->setEnabled(enable);
}

void ShoeModelView::refreshComboBoxes()
{
	ViewHelper::fillComboBox(shoeMakerComboBox, Objects::Types::ShoeMaker);
}
