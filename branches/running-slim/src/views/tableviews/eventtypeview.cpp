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
#include "eventtypeview.h"
#include "delegates/booleanimageitemdelegate.h"
#include "utility/completerhelper.h"

EventTypeView::EventTypeView(QWidget *parent, quint32 id)
	: AbstractTableView(parent)
{
	setupUi(this);

	m_model->setTable("EventType");
	m_model->select();

	m_tableView = tableView;

	tableView->setModel(m_model);
	tableView->hideColumn(0);
	tableView->setColumnWidth(1, 170);
	tableView->setColumnWidth(2, 80);
	tableView->setItemDelegateForColumn(2, new BooleanImageItemDelegate(":images/medal", tableView));
	tableView->setColumnWidth(3, 80);
	tableView->setItemDelegateForColumn(3, new BooleanImageItemDelegate(":images/intervals", tableView));

	connect(tableView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(currentRowChanged(const QModelIndex &, const QModelIndex &)));

	descriptionLineEdit->setCompleter(CompleterHelper::completer("EventType", "Description", descriptionLineEdit));

	refresh(id);
}

EventTypeView::~EventTypeView()
{
}

void EventTypeView::currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(previous);

	descriptionLineEdit->setText(current.sibling(current.row(), 1).data().toString());
	hasMedalCheckBox->setChecked(current.sibling(current.row(), 2).data().toBool());
	hasIntervalsCheckBox->setChecked(current.sibling(current.row(), 3).data().toBool());
}

void EventTypeView::on_descriptionLineEdit_textChanged(const QString &value)
{
	QModelIndex index = tableView->currentIndex();
	QString description = index.sibling(index.row(), 1).data().toString();
	if (value != description) {
		m_model->setData(index.sibling(index.row(), 1), value);
	}
}

void EventTypeView::on_hasMedalCheckBox_stateChanged(int value)
{
	bool state = value > 0;
	QModelIndex index = tableView->currentIndex();
	bool hasMedal = index.sibling(index.row(), 2).data().toBool();
	if (state != hasMedal) {
		m_model->setData(index.sibling(index.row(), 2), state);
	}
}

void EventTypeView::on_hasIntervalsCheckBox_stateChanged(int value)
{
	bool state = value > 0;
	QModelIndex index = tableView->currentIndex();
	bool hasIntervals = index.sibling(index.row(), 3).data().toBool();
	if (state != hasIntervals) {
		m_model->setData(index.sibling(index.row(), 3), state);
	}
}

void EventTypeView::setControlsEnabled(bool value)
{
	descriptionLineEdit->setEnabled(value);
	hasMedalCheckBox->setEnabled(value);
	hasIntervalsCheckBox->setEnabled(value);
}
