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
	m_type = type;

	QList<Objects::BaseObject *> objects = Application::instance()->objectMap()->getAllObjects(m_type);
	foreach (Objects::BaseObject *object, objects) {
		m_objects << static_cast<Objects::ComboObject *>(object);
	}
}

ComboObjectItemDelegate::~ComboObjectItemDelegate()
{
	foreach (Objects::ComboObject *object, m_objects) {
		Application::instance()->objectMap()->discardObject(object);
	}
}



void ComboObjectItemDelegate::refreshItems()
{
	foreach (Objects::ComboObject *object, m_objects) {
		Application::instance()->objectMap()->discardObject(object);
	}

	m_objects.clear();

	QList<Objects::BaseObject *> objects = Application::instance()->objectMap()->getAllObjects(m_type);
	foreach (Objects::BaseObject *object, objects) {
		m_objects << static_cast<Objects::ComboObject *>(object);
	}
}



void ComboObjectItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	quint32 value = index.model()->data(index, Qt::DisplayRole).toInt();

	drawBackground(painter, option, index);

	QString description = "";
	foreach (Objects::ComboObject *object, m_objects) {
		if (object->id() == value) {
			description = object->description();
		}
	}
	drawDisplay(painter, option, option.rect, description);

	drawFocus(painter, option, option.rect);
}

QSize ComboObjectItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);

	return QSize();
}

QWidget *ComboObjectItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &index) const
{
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
