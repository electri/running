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

	m_pendingDataChanges = true;

	refresh();
}

void AbstractTableView::on_removePushButton_clicked()
{
	if (m_model->rowCount() == 0) {
		return;
	}

	QModelIndexList indexes = m_tableView->selectionModel()->selectedIndexes();
	if (indexes.isEmpty()) {
		return;
	}

	int row = indexes.at(0).row();
	m_model->removeRows(row, 1);

	m_pendingDataChanges = true;

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
	int rowCount = m_model->rowCount();
	int columnCount = m_model->columnCount();
	for (int i = 0; i < rowCount; ++i) {
		bool rowDirty = false;
		QString s = QString("[SUBMIT] %1: ").arg(i);
		for (int y = 0; y < columnCount; ++y) {
			QModelIndex index = m_model->index(i, y);
			if (m_model->isDirty(index)) rowDirty = true;
			s += index.data().toString();
			if (y != 0) s += ";";
		}
		if (rowDirty) qDebug() << s;
	}

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
	Q_UNUSED(topLeft);
	Q_UNUSED(bottomRight);

	m_pendingDataChanges = true;
}

void AbstractTableView::setControlsEnabled(bool value)
{
	Q_UNUSED(value);
}

void AbstractTableView::refresh(int id)
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

void AbstractTableView::swapRows(int row1, int row2)
{
	int rowCount = m_model->rowCount();
	if (rowCount < 2) return;
	if ((row1 < 0) || (row2 < 0)) return;
	if ((row1 > (rowCount - 1)) || (row2 > (rowCount - 1))) return;

	QVariant value;
	int columnCount = m_model->columnCount();
	for (int i = 1; i <= columnCount; ++i) {
		value = m_model->index(row1, i).data();
		m_model->setData(m_model->index(row1, i), m_model->index(row2, i).data());
		m_model->setData(m_model->index(row2, i), value);
	}
}

void AbstractTableView::onBoolChanged(int column, int state)
{
	bool value = state > 0;

	QModelIndex index = m_tableView->currentIndex();
	if (index.isValid()) {
		bool data = index.sibling(index.row(), column).data().toBool();
		if (value != data) {
			m_model->setData(index.sibling(index.row(), column), value);
		}
	}
}

void AbstractTableView::onIntChanged(int column, int value)
{
	QModelIndex index = m_tableView->currentIndex();
	if (index.isValid()) {
		int data = index.sibling(index.row(), column).data().toInt();
		if (value != data) {
			m_model->setData(index.sibling(index.row(), column), value);
		}
	}
}

void AbstractTableView::onDoubleChanged(int column, double value)
{
	QModelIndex index = m_tableView->currentIndex();
	if (index.isValid()) {
		double data = index.sibling(index.row(), column).data().toDouble();
		if (value != data) {
			m_model->setData(index.sibling(index.row(), column), value);
		}
	}
}

void AbstractTableView::onDateChanged(int column, const QDate &value)
{
	QModelIndex index = m_tableView->currentIndex();
	if (index.isValid()) {
		QDate description = index.sibling(index.row(), column).data().toDate();
		if (value != description) {
			m_model->setData(index.sibling(index.row(), column), value);
		}
	}
}

void AbstractTableView::onTimeChanged(int column, const QTime &value)
{
	QModelIndex index = m_tableView->currentIndex();
	if (index.isValid()) {
		QTime description = index.sibling(index.row(), column).data().toTime();
		if (value != description) {
			m_model->setData(index.sibling(index.row(), column), value);
		}
	}
}

void AbstractTableView::onTextChanged(int column, const QString &value)
{
	QModelIndex index = m_tableView->currentIndex();
	if (index.isValid()) {
		QString description = index.sibling(index.row(), column).data().toString();
		if (value != description) {
			m_model->setData(index.sibling(index.row(), column), value);
		}
	}
}

void AbstractTableView::onPlainTextChanged(int column, const QPlainTextEdit *widget)
{
	QString value = widget->toPlainText();

	QModelIndex index = m_tableView->currentIndex();
	if (index.isValid()) {
		QString data = index.sibling(index.row(), column).data().toString();
		if (value != data) {
			m_model->setData(index.sibling(index.row(), column), value);
		}
	}
}
