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
#include "intervalpopupview.h"
#include "settings.h"
#include "objects/eventgateway.h"
#include "delegates/comboboxitemdelegate.h"
#include "delegates/distancestyleditemdelegate.h"
#include "delegates/durationstyleditemdelegate.h"
#include "views/tableviews/intervaltypeview.h"
#include "utility/comboboxhelper.h"
#include "utility/utility.h"

IntervalPopupView::IntervalPopupView(EventGateway *event, QWidget *parent)
	: AbstractTableView(parent)
{
	setupUi(this);

	m_event = event;

	m_model->setTable("Interval");
	m_model->setFilter(QString("EventId = %1").arg(m_event->id()));
	m_model->select();

	m_model->setHeaderData(2, Qt::Horizontal, tr("Type"), Qt::DisplayRole);
	m_model->setHeaderData(3, Qt::Horizontal, tr("Distance"), Qt::DisplayRole);
	m_model->setHeaderData(4, Qt::Horizontal, tr("Duration"), Qt::DisplayRole);

	m_tableView = tableView;

	tableView->setModel(m_model);
	tableView->hideColumn(0);
	tableView->hideColumn(1);
	tableView->setColumnWidth(2, 130);
	tableView->setItemDelegateForColumn(2, new ComboBoxItemDelegate("IntervalType", tableView));
	tableView->setColumnWidth(3, 100);
	tableView->setItemDelegateForColumn(3, new DistanceStyledItemDelegate(3, tableView));
	tableView->setColumnWidth(4, 100);
	tableView->setItemDelegateForColumn(4, new DurationStyledItemDelegate(tableView));
	tableView->hideColumn(5);

	ComboBoxHelper::fillComboBox(intervalTypeComboBox, "IntervalType", false);

	connect(tableView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(currentRowChanged(const QModelIndex &, const QModelIndex &)));
	connect(m_model, SIGNAL(beforeInsert(QSqlRecord&)), this, SLOT(beforeInsert(QSqlRecord&)));

	refresh(0);
}

IntervalPopupView::~IntervalPopupView()
{
}

void IntervalPopupView::showEvent(QShowEvent *event)
{
	Q_UNUSED(event);

	tableView->setItemDelegateForColumn(3, new DistanceStyledItemDelegate(3, tableView));
	distanceDoubleSpinBox->setSuffix(" " + Settings::instance()->distanceUnit());

	paceLineEdit->setText(Utility::formatPace(distanceDoubleSpinBox->value(), durationTimeEdit->time()));
}

QSqlTableModel *IntervalPopupView::model() const
{
	return m_model;
}

void IntervalPopupView::resetAll()
{
	m_model->revertAll();
	m_pendingDataChanges = false;

	refresh();
}

bool IntervalPopupView::saveAll()
{
	// set defaults
	int rowCount = m_model->rowCount();
//	int columnCount = m_model->columnCount();
	for (int i = 0; i < rowCount; ++i) {
		// event id
		QModelIndex index = m_model->index(i, 1);
		if (index.data().isNull()) {
			m_model->setData(index, m_event->id());
		}

//		bool rowDirty = false;
//		QString s = QString("[SUBMIT] %1: ").arg(i);
//		for (int y = 0; y < columnCount; ++y) {
//			QModelIndex index1 = m_model->index(i, y);
//			if (m_model->isDirty(index1)) rowDirty = true;
//			if (y != 0) s += ";";
//			s += index1.data().toString();
//		}
//		if (rowDirty) qDebug() << s;
	}

	return m_model->submitAll();
}

void IntervalPopupView::currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(previous);

	ComboBoxHelper::setSelected(intervalTypeComboBox, current.sibling(current.row(), 2).data().toInt());
	distanceDoubleSpinBox->setValue(current.sibling(current.row(), 3).data().toDouble());
	durationTimeEdit->setTime(current.sibling(current.row(), 4).data().toTime());
	notesPlainTextEdit->setPlainText(current.sibling(current.row(), 5).data().toString());

	paceLineEdit->setText(Utility::formatPace(distanceDoubleSpinBox->value(), durationTimeEdit->time()));
}

void IntervalPopupView::on_rowUpPushButton_clicked()
{
	swapRows(tableView->currentIndex().row(), tableView->currentIndex().row() - 1);
}

void IntervalPopupView::on_rowDownPushButton_clicked()
{
	swapRows(tableView->currentIndex().row(), tableView->currentIndex().row() + 1);
}

void IntervalPopupView::on_intervalTypeComboBox_currentIndexChanged(int id)
{
	int value = intervalTypeComboBox->itemData(id).toInt();
	onIntChanged(2, value);
}

void IntervalPopupView::on_distanceDoubleSpinBox_valueChanged(double value)
{
	onDoubleChanged(3, value);

	paceLineEdit->setText(Utility::formatPace(value, durationTimeEdit->time()));
}

void IntervalPopupView::on_durationTimeEdit_timeChanged(const QTime &value)
{
	onTimeChanged(4, value);

	paceLineEdit->setText(Utility::formatPace(distanceDoubleSpinBox->value(), value));
}

void IntervalPopupView::on_notesPlainTextEdit_textChanged()
{
	onPlainTextChanged(5, notesPlainTextEdit);
}

void IntervalPopupView::on_intervalTypeToolButton_clicked()
{
	IntervalTypeView view;
	int result = view.exec();
	if (result == QDialog::Accepted) {
		ComboBoxHelper::fillComboBox(intervalTypeComboBox, "IntervalType", false);

		ComboBoxItemDelegate *delegate = qobject_cast<ComboBoxItemDelegate *>(tableView->itemDelegateForColumn(2));
		if (delegate) {
			delegate->refreshItems();
		}
	}
}

void IntervalPopupView::setControlsEnabled(bool enable)
{
	intervalTypeComboBox->setEnabled(enable);
	distanceDoubleSpinBox->setEnabled(enable);
	durationTimeEdit->setEnabled(enable);
	notesPlainTextEdit->setEnabled(enable);
}
