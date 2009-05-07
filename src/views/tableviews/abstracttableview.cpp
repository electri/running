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
#include "abstracttableview.h"

AbstractTableView::AbstractTableView(QWidget *parent)
	: QDialog(parent)
{
	m_model = new QSqlTableModel(this);
	m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

	m_pendingDataChanges = false;

	connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(dataChanged(QModelIndex,QModelIndex)));
}

AbstractTableView::~AbstractTableView()
{
	delete m_model;
}

void AbstractTableView::on_addPushButton_clicked()
{
	int row = m_model->rowCount() > 0 ? m_tableView->currentIndex().row() + 1 : 0;
	m_model->insertRows(row, 1);

	refresh();
}

void AbstractTableView::on_removePushButton_clicked()
{
	if (m_model->rowCount() > 0) {
		QModelIndexList indexes = m_tableView->selectionModel()->selectedIndexes();
		if (!indexes.isEmpty()) {
			m_model->removeRows(indexes.at(0).row(), 1);
		}
	}

	refresh();
}

void AbstractTableView::on_resetPushButton_clicked()
{
	if (m_pendingDataChanges) {
		if (QMessageBox::question(this, tr("Reset"), tr("There are pending changes to save. Are you sure to reset?"), QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes) return;
	}

	m_model->revertAll();
	m_pendingDataChanges = false;

	refresh();
}

void AbstractTableView::on_savePushButton_clicked()
{
	bool rc = m_model->submitAll();
	if (!rc) {
		QMessageBox::critical(this, tr("Error"), m_model->lastError().text());
		return;
	}

	accept();
}

void AbstractTableView::on_cancelPushButton_clicked()
{
	if (m_pendingDataChanges) {
		if (QMessageBox::question(this, tr("Cancel"), tr("There are pending changes to save. Are you sure to cancel?"), QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes) return;
	}

	reject();
}

void AbstractTableView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
	m_pendingDataChanges = true;
}

void AbstractTableView::setControlsEnabled(bool value)
{
	Q_UNUSED(value);
}

void AbstractTableView::refresh(quint32 id)
{
	if (m_model->rowCount() > 0) {
		QModelIndexList list = m_model->match(m_model->index(0, 0), Qt::DisplayRole, id, 1, Qt::MatchExactly | Qt::MatchWrap);
		if (!list.isEmpty()) {
			m_tableView->setCurrentIndex(m_model->index(list[0].row(), 1));
		} else {
			m_tableView->setCurrentIndex(m_model->index(0, 1));
		}

		setControlsEnabled(true);
	} else {
		setControlsEnabled(false);
	}
}
