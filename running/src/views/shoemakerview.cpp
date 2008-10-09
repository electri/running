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

#include "shoemakerview.h"

#include "../models/comboobjecttablemodel.h"
#include "../services/objectrepository.h"

ShoeMakerView::ShoeMakerView(QWidget *parent, quint32 id)
	: QDialog(parent)
{
	setupUi(this);

	m_model = new ComboObjectTableModel(Objects::Types::ShoeMaker, this);

	tableView->setModel(m_model);
	tableView->hideColumn(0);
	tableView->setColumnWidth(1, 330);

	connect(tableView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
									 this, SLOT(currentRowChanged(const QModelIndex &, const QModelIndex &)));

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

ShoeMakerView::~ShoeMakerView()
{
	delete m_model;
}



void ShoeMakerView::on_addPushButton_clicked()
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

void ShoeMakerView::on_removePushButton_clicked()
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

void ShoeMakerView::on_resetPushButton_clicked()
{
	m_model->revertAll();
	if (m_model->rowCount() > 0) {
		tableView->setCurrentIndex(m_model->index(0, 1));
		this->setControlsEnabled(true);
	} else {
		this->setControlsEnabled(false);
	}
}

void ShoeMakerView::on_savePushButton_clicked()
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

void ShoeMakerView::on_cancelPushButton_clicked()
{
	this->reject();
}



void ShoeMakerView::currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
	descriptionLineEdit->setText(current.sibling(current.row(), 1).data().toString());
}



void ShoeMakerView::on_descriptionLineEdit_textChanged(const QString &text)
{
	tableView->model()->setData(tableView->currentIndex().sibling(tableView->currentIndex().row(), 1), text);
}



void ShoeMakerView::setControlsEnabled(bool enable)
{
	descriptionLineEdit->setEnabled(enable);
}
