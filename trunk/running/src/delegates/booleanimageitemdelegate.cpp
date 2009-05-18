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
#include "booleanimageitemdelegate.h"

BooleanImageItemDelegate::BooleanImageItemDelegate(const QString &image, QObject *parent)
	: QItemDelegate(parent)
{
	QIcon icon = QIcon(image);
	m_image_on = icon.pixmap(16, 16);
	m_image_off = icon.pixmap(16, 16, QIcon::Disabled);
}

void BooleanImageItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	drawBackground(painter, option, index);
	drawFocus(painter, option, option.rect);

	int value = index.model()->data(index, Qt::DisplayRole).toBool();

	QPoint point = option.rect.topLeft() + QPoint((option.rect.width() - m_image_on.width()) / 2,
					(option.rect.height() - m_image_on.height()) / 2	);
	if (value) {
		painter->drawPixmap(point, m_image_on);
	} else {
		painter->drawPixmap(point, m_image_off);
	}
}

QSize BooleanImageItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);

	return m_image_on.size();
}

QWidget *BooleanImageItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);

	return QItemEditorFactory::defaultFactory()->createEditor(QVariant::Bool, parent);
}

void BooleanImageItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	int value = index.model()->data(index, Qt::EditRole).toInt();

	QByteArray propertyName = QItemEditorFactory::defaultFactory()->valuePropertyName(QVariant::Bool);
	editor->setProperty(propertyName, value == 1);
}

void BooleanImageItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QByteArray propertyName = QItemEditorFactory::defaultFactory()->valuePropertyName(QVariant::Bool);
	int value = editor->property(propertyName).toBool() ? 1 : 0;

	model->setData(index, value, Qt::EditRole);
}

void BooleanImageItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index);

	editor->setGeometry(option.rect);
}
