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
#include "comboboxitemdelegate.h"

ComboBoxItemDelegate::ComboBoxItemDelegate(const QString &tableName, QObject *parent)
	: QItemDelegate(parent)
{
	m_tableName = tableName;

	refreshItems();
}

ComboBoxItemDelegate::~ComboBoxItemDelegate()
{
}

void ComboBoxItemDelegate::refreshItems()
{
	m_objects.clear();

	QString queryText = QString("SELECT Id, Description FROM %1 ORDER BY Description").arg(m_tableName);
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isValid()) {
		if (db.isOpen()) {
			QSqlQuery query(db);
			query.exec(queryText);
			while (query.next()) {
				int id = query.value(0).toInt();
				QString description = query.value(1).toString();
				m_objects[id] = description;
			}
		}
	}
}



void ComboBoxItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	int id = index.model()->data(index, Qt::DisplayRole).toInt();
	QString description = "";
	if (m_objects.contains(id)) {
		description = m_objects[id];
	}

	drawBackground(painter, option, index);
	drawDisplay(painter, option, option.rect, description);
	drawFocus(painter, option, option.rect);
}

QSize ComboBoxItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);

	return QSize();
}

/*
QWidget *ComboObjectItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);

	QComboBox *editor = new QComboBox(parent);

	foreach (Objects::ComboObject *object, m_objects) {
		editor->addItem(object->description(), object->id());
	}

	return editor;
}

void ComboObjectItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	quint32 value = index.model()->data(index, Qt::EditRole).toInt();

	QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
	comboBox->setCurrentIndex(comboBox->findData(value));
}

void ComboObjectItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
	quint32 value = comboBox->itemData(comboBox->currentIndex()).toInt();

	model->setData(index, value, Qt::EditRole);
}

void ComboObjectItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index);

	editor->setGeometry(option.rect);
}
*/
