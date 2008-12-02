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

#include "intervalview.h"

#include "../application.h"
#include "../objects/event.h"
#include "../objects/interval.h"
#include "../objects/intervaltype.h"
#include "../models/intervaltablemodel.h"
#include "../delegates/comboobjectitemdelegate.h"
#include "../delegates/distancestyleditemdelegate.h"
#include "../delegates/durationstyleditemdelegate.h"
#include "../views/intervaltypeview.h"
#include "../views/viewhelper.h"
#include "../utility/utility.h"

IntervalView::IntervalView(Objects::Event *event, QWidget *parent, quint32 id)
	: QWidget(parent)
{
	setupUi(this);

	m_model = new IntervalTableModel(event, this);

	tableView->setModel(m_model);
	tableView->hideColumn(0);
	tableView->setColumnWidth(1, 130);
	tableView->setItemDelegateForColumn(1, new ComboObjectItemDelegate(Objects::Types::IntervalType, tableView));
	tableView->setColumnWidth(2, 100);
	tableView->setItemDelegateForColumn(2, new DistanceStyledItemDelegate(tableView, 3, "", " " + Application::instance()->cfg()->cfgDistanceUnit()->description()));
	tableView->setColumnWidth(3, 100);
	tableView->setItemDelegateForColumn(3, new DurationStyledItemDelegate(tableView));
	tableView->hideColumn(4);

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

IntervalView::~IntervalView()
{
	delete m_model;
}

void IntervalView::showEvent(QShowEvent *event)
{
	Q_UNUSED(event);

	tableView->setItemDelegateForColumn(2, new DistanceStyledItemDelegate(tableView, 3, "", " " + Application::instance()->cfg()->cfgDistanceUnit()->description()));

	distanceDoubleSpinBox->setSuffix(" " + Application::instance()->cfg()->cfgDistanceUnit()->description());

	this->refreshPaceLineEdit(distanceDoubleSpinBox->value(), durationTimeEdit->time());
}



void IntervalView::on_addPushButton_clicked()
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

void IntervalView::on_removePushButton_clicked()
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

void IntervalView::resetAll()
{
	m_model->revertAll();
	if (m_model->rowCount() > 0) {
		tableView->setCurrentIndex(m_model->index(0, 1));
		this->setControlsEnabled(true);
	} else {
		this->setControlsEnabled(false);
	}
}

bool IntervalView::saveAll()
{
	bool result = m_model->submitAll();
	if (!result) {
		return false;
	}
	return true;
}



void IntervalView::on_rowUpPushButton_clicked()
{
	if (m_model->rowCount() < 2) return;

	if (tableView->currentIndex().row() == 0) return;

	this->swapRows(tableView->currentIndex().row(), tableView->currentIndex().row() - 1);
}

void IntervalView::on_rowDownPushButton_clicked()
{
	if (m_model->rowCount() < 2) return;

	if (tableView->currentIndex().row() == (m_model->rowCount() - 1)) return;

	this->swapRows(tableView->currentIndex().row(), tableView->currentIndex().row() + 1);
}



void IntervalView::currentRowChanged(const QModelIndex &current, const QModelIndex &)
{
//	if (intervalTypeComboBox->findData(current.sibling(current.row(), 1).data().toInt()) != -1) {
//		intervalTypeComboBox->setCurrentIndex(intervalTypeComboBox->findData(current.sibling(current.row(), 1).data().toInt()));
//	} else {
//		intervalTypeComboBox->setCurrentIndex(0);
//	}
	intervalTypeComboBox->setCurrentIndex(intervalTypeComboBox->findData(current.sibling(current.row(), 1).data().toInt()));
	distanceDoubleSpinBox->setValue(current.sibling(current.row(), 2).data().toDouble());
	durationTimeEdit->setTime(current.sibling(current.row(), 3).data().toTime());
	notesPlainTextEdit->setPlainText(current.sibling(current.row(), 4).data().toString());
}



void IntervalView::on_intervalTypeComboBox_currentIndexChanged(int index)
{
	quint32 intervalTypeId = intervalTypeComboBox->itemData(index).toInt();
	m_model->setData(tableView->currentIndex().sibling(tableView->currentIndex().row(), 1), intervalTypeId);
}

void IntervalView::on_distanceDoubleSpinBox_valueChanged(double value)
{
	m_model->setData(tableView->currentIndex().sibling(tableView->currentIndex().row(), 2), value);

	this->refreshPaceLineEdit(value, durationTimeEdit->time());
}

void IntervalView::on_durationTimeEdit_timeChanged(const QTime &value)
{
	m_model->setData(tableView->currentIndex().sibling(tableView->currentIndex().row(), 3), value);

	this->refreshPaceLineEdit(distanceDoubleSpinBox->value(), value);
}

void IntervalView::on_notesPlainTextEdit_textChanged()
{
	m_model->setData(tableView->currentIndex().sibling(tableView->currentIndex().row(), 4), notesPlainTextEdit->toPlainText());
}



void IntervalView::on_intervalTypeToolButton_clicked()
{
	IntervalTypeView *view = new IntervalTypeView(this);
	int result = view->exec();
	if (result == QDialog::Accepted) {
		quint32 intervalTypeId = intervalTypeComboBox->itemData(intervalTypeComboBox->currentIndex()).toInt();
		this->refreshComboBoxes();
		intervalTypeComboBox->setCurrentIndex(intervalTypeComboBox->findData(intervalTypeId));
	}
	delete view;
}



void IntervalView::setControlsEnabled(bool enable)
{
	intervalTypeComboBox->setEnabled(enable);
	distanceDoubleSpinBox->setEnabled(enable);
	durationTimeEdit->setEnabled(enable);
	notesPlainTextEdit->setEnabled(enable);
}

void IntervalView::refreshComboBoxes()
{
	ViewHelper::fillComboBox(intervalTypeComboBox, Objects::Types::IntervalType, false);
}

void IntervalView::swapRows(int row1, int row2)
{
	QVariant value;
	for (int i = 1; i <= 4; ++i) {
		value = m_model->index(row1, i).data();
		m_model->setData(m_model->index(row1, i), m_model->index(row2, i).data());
		m_model->setData(m_model->index(row2, i), value);
	}
}

void IntervalView::refreshPaceLineEdit(double distance, QTime time)
{
	QTime paceTime = Utility::paceTime(distance, time);
	double paceSpeed = Utility::paceSpeed(distance, time);
	paceLineEdit->setText(tr("%1 min/%3 or %2 %3/h")
			.arg(Utility::formatDuration(paceTime)).arg(Utility::formatDistance(paceSpeed, 2))
			.arg(Application::instance()->cfg()->cfgDistanceUnit()->description()));
}
