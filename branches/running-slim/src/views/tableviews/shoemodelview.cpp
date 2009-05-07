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
#include "shoemodelview.h"
#include "delegates/comboboxitemdelegate.h"
#include "utility/comboboxhelper.h"
#include "utility/completerhelper.h"
#include "views/tableviews/shoemakerview.h"

ShoeModelView::ShoeModelView(QWidget *parent, quint32 id)
	: AbstractTableView(parent)
{
	setupUi(this);

	m_model->setTable("ShoeModel");
	m_model->select();

	m_tableView = tableView;

	tableView->setModel(m_model);
	tableView->hideColumn(0);
	tableView->setColumnWidth(1, 200);
	tableView->setColumnWidth(2, 130);
	tableView->setItemDelegateForColumn(2, new ComboBoxItemDelegate("ShoeMaker", tableView));

	QHeaderView *headerView = tableView->horizontalHeader();
	headerView->swapSections(2, 1);

	connect(tableView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(currentRowChanged(const QModelIndex &, const QModelIndex &)));

	descriptionLineEdit->setCompleter(CompleterHelper::completer("ShoeModel", "Description", descriptionLineEdit));

	ComboBoxHelper::fillComboBox(shoeMakerComboBox, "ShoeMaker", false);

	refresh(id);
}

ShoeModelView::~ShoeModelView()
{
}

void ShoeModelView::currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(previous);

	ComboBoxHelper::setSelected(shoeMakerComboBox, current.sibling(current.row(), 2).data().toInt());
	descriptionLineEdit->setText(current.sibling(current.row(), 1).data().toString());
}

void ShoeModelView::on_shoeMakerComboBox_currentIndexChanged(int id)
{
	int value = shoeMakerComboBox->itemData(id).toInt();

	QModelIndex index = tableView->currentIndex();
	int data = index.sibling(index.row(), 2).data().toInt();
	if (value != data) {
		m_model->setData(index.sibling(index.row(), 2), value);
	}
}

void ShoeModelView::on_descriptionLineEdit_textChanged(const QString &value)
{
	QModelIndex index = tableView->currentIndex();
	QString description = index.sibling(index.row(), 1).data().toString();
	if (value != description) {
		m_model->setData(index.sibling(index.row(), 1), value);
	}
}

void ShoeModelView::on_shoeMakerToolButton_clicked()
{
	int id = ComboBoxHelper::selected(shoeMakerComboBox);

	ShoeMakerView view(this, id);
	int result = view.exec();
	if (result == QDialog::Accepted) {
		ComboBoxHelper::fillComboBox(shoeMakerComboBox, "ShoeMaker", false);

		ComboBoxItemDelegate *delegate = qobject_cast<ComboBoxItemDelegate *>(tableView->itemDelegateForColumn(1));
		delegate->refreshItems();
	}
}

void ShoeModelView::setControlsEnabled(bool enable)
{
	shoeMakerComboBox->setEnabled(enable);
	descriptionLineEdit->setEnabled(enable);
}
