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
#include "intervaltypeview.h"
#include "utility/completerhelper.h"

IntervalTypeView::IntervalTypeView(QWidget *parent, quint32 id)
	: AbstractTableView(parent)
{
	setupUi(this);

	m_model->setTable("IntervalType");
	m_model->select();

	m_tableView = tableView;

	tableView->setModel(m_model);
	tableView->hideColumn(0);
	tableView->setColumnWidth(1, 330);

	connect(tableView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(currentRowChanged(const QModelIndex &, const QModelIndex &)));

	descriptionLineEdit->setCompleter(CompleterHelper::completer("IntervalType", "Description", descriptionLineEdit));

	refresh(id);
}

IntervalTypeView::~IntervalTypeView()
{
}

void IntervalTypeView::currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(previous);

	descriptionLineEdit->setText(current.sibling(current.row(), 1).data().toString());
}

void IntervalTypeView::on_descriptionLineEdit_textChanged(const QString &value)
{
	QModelIndex index = tableView->currentIndex();
	QString description = index.sibling(index.row(), 1).data().toString();
	if (value != description) {
		m_model->setData(index.sibling(index.row(), 1), value);
	}
}

void IntervalTypeView::setControlsEnabled(bool enable)
{
	descriptionLineEdit->setEnabled(enable);
}
