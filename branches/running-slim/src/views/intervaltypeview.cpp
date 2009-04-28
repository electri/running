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

#include "intervaltypeview.h"

#include "../models/comboobjecttablemodel.h"
#include "../views/viewhelper.h"

IntervalTypeView::IntervalTypeView(QWidget *parent, quint32 id)
	: QDialog(parent)
{
	setupUi(this);

	m_model = new ComboObjectTableModel(Objects::Types::IntervalType, this);

	tableView->setModel(m_model);
	tableView->hideColumn(0);
	tableView->setColumnWidth(1, 330);

	connect(tableView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(currentRowChanged(const QModelIndex &, const QModelIndex &)));

	descriptionLineEdit->setCompleter(ViewHelper::completer(descriptionLineEdit, "IntervalType", "Description"));

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

IntervalTypeView::~IntervalTypeView()
{
	delete m_model;
}



void IntervalTypeView::on_addPushButton_clicked()
{
	if (m_model->rowCount() == 0) {
		this->setControlsEnabled(true);
	}

	int row = m_model->rowCount() > 0 ? tableView->currentIndex().row() + 1 : 0;
	m_model->insertRows(row, 1);
	tableView->setCurrentIndex(m_model->index(row, 1));
}

void IntervalTypeView::on_removePushButton_clicked()
{
	if (m_model->rowCount() == 1) {
		this->setControlsEnabled(false);
	}

	if (m_model->rowCount() > 0) {
		QModelIndexList indexes = tableView->selectionModel()->selectedIndexes();
		if (!indexes.isEmpty()) {
			m_model->removeRows(indexes.at(0).row(), 1);
		}
	}
}

void IntervalTypeView::on_resetPushButton_clicked()
{
	m_model->revertAll();

	if (m_model->rowCount() > 0) {
		this->setControlsEnabled(true);
		tableView->setCurrentIndex(m_model->index(0, 1));
	} else {
		this->setControlsEnabled(false);
	}
}

void IntervalTypeView::on_savePushButton_clicked()
{
	bool result = m_model->submitAll();
	if (!result) {
		QMessageBox::critical(this, tr("Error"), m_model->lastError());
		return;
	}

	this->accept();
}

void IntervalTypeView::on_cancelPushButton_clicked()
{
	this->reject();
}



void IntervalTypeView::currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(previous);

	descriptionLineEdit->setText(current.sibling(current.row(), 1).data().toString());
}



void IntervalTypeView::on_descriptionLineEdit_textChanged(const QString &text)
{
	tableView->model()->setData(tableView->currentIndex().sibling(tableView->currentIndex().row(), 1), text);
}



void IntervalTypeView::setControlsEnabled(bool enable)
{
	descriptionLineEdit->setEnabled(enable);
}
