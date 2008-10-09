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

#include "calendardelegate.h"
#include "../objects/event.h"
#include "../objects/eventtype.h"
#include "../utility/utility.h"

CalendarDelegate::CalendarDelegate(QObject *parent)
	: QItemDelegate(parent)
{
	m_medal = QImage(":medal");
	m_vote_high = QImage(":vote_high");
	m_vote_low = QImage(":vote_low");
}

void CalendarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QDate &date) const
{
	painter->save();

	drawBackground(painter, option, QModelIndex());
	drawDisplay(painter, option, option.rect, date.toString("d"));
	drawFocus(painter, option, option.rect);

	painter->restore();
}

void CalendarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QDate &date, Objects::Event *event) const
{
	painter->save();

	drawBackground(painter, option, QModelIndex());
	drawDisplay(painter, option, option.rect, date.toString("d"));
	drawFocus(painter, option, option.rect);

	QFont font = painter->font();
	font.setBold(true);
	painter->setFont(font);

	QString type = event->eventType()->description();
	type = type.toLower();
	QRect rect1 = painter->boundingRect(option.rect, Qt::AlignCenter | Qt::TextWordWrap, type);
	painter->drawText(option.rect, Qt::AlignCenter | Qt::TextWordWrap, type);

	font.setBold(false);
	painter->setFont(font);

	QString distance = Utility::formatDistance(event->distance(), 3) + " km";
	QRect rect2 = painter->boundingRect(option.rect, Qt::TextSingleLine, distance);
	rect2.moveCenter(option.rect.center());
	rect2.moveTop(rect1.bottom());
	painter->drawText(rect2, Qt::TextSingleLine, distance);

	int x = option.rect.right();
	if (event->eventType()->hasMedal()) {
		x -= (m_medal.width() + 2);
		painter->drawImage(QPoint(x, option.rect.top() + 2), m_medal);
	}
	if (event->vote() >= 5) {
		x -= (m_vote_high.width() + 2);
		painter->drawImage(QPoint(x, option.rect.top() + 2), m_vote_high);
	}
	if (event->vote() <= 1) {
		x -= (m_vote_low.width() + 2);
		painter->drawImage(QPoint(x, option.rect.top() + 2), m_vote_low);
	}

	painter->restore();
}
