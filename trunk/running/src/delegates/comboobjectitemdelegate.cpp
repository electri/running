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

#include "comboobjectitemdelegate.h"

#include "../application.h"

ComboObjectItemDelegate::ComboObjectItemDelegate(Objects::Types::Type type, QObject *parent)
	: QItemDelegate(parent)
{
	m_list = Application::instance()->objectMap()->getAllObjects(type);
}

ComboObjectItemDelegate::~ComboObjectItemDelegate()
{
	Application::instance()->objectMap()->discardObjects(m_list);
}

void ComboObjectItemDelegate::paint(QPainter *painter,
		const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	quint32 value = index.model()->data(index, Qt::DisplayRole).toInt();

	drawBackground(painter, option, index);

	QString description = "";
	QList<Objects::BaseObject *>::const_iterator it = m_list.constBegin();
	while (it != m_list.constEnd()) {
		Objects::ComboObject *item = static_cast<Objects::ComboObject *>(*it++);
		if (item->id() == value) {
			description = item->description();
		}
	}
	drawDisplay(painter, option, option.rect, description);

	drawFocus(painter, option, option.rect);
}

QSize ComboObjectItemDelegate::sizeHint(const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
	return QSize();
}

QWidget *ComboObjectItemDelegate::createEditor(QWidget *parent,
		const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
	QComboBox *editor = new QComboBox(parent);

	QList<Objects::BaseObject *>::const_iterator it = m_list.constBegin();
	while (it != m_list.constEnd()) {
		Objects::ComboObject *item = static_cast<Objects::ComboObject *>(*it++);
		editor->addItem(item->description(), item->id());
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

void ComboObjectItemDelegate::updateEditorGeometry(QWidget *editor,
			const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}
