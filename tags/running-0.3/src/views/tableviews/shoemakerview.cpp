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
#include "shoemakerview.h"
#include "utility/completerhelper.h"

ShoeMakerView::ShoeMakerView(QWidget *parent, quint32 id)
	: AbstractTableView(parent)
{
	setupUi(this);

	m_model->setTable("ShoeMaker");
	m_model->select();

	m_model->setHeaderData(1, Qt::Horizontal, tr("Description"), Qt::DisplayRole);

	m_tableView = tableView;

	tableView->setModel(m_model);
	tableView->hideColumn(0);
	tableView->setColumnWidth(1, 330);

	connect(tableView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(currentRowChanged(const QModelIndex &, const QModelIndex &)));

	descriptionLineEdit->setCompleter(CompleterHelper::completer("ShoeMaker", "Description", descriptionLineEdit));

	refresh(id);
}

ShoeMakerView::~ShoeMakerView()
{
}

void ShoeMakerView::currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(previous);

	descriptionLineEdit->setText(current.sibling(current.row(), 1).data().toString());
}

void ShoeMakerView::on_descriptionLineEdit_textChanged(const QString &value)
{
	onTextChanged(1, value);
}

void ShoeMakerView::setControlsEnabled(bool enable)
{
	descriptionLineEdit->setEnabled(enable);
}
