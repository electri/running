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
#include "calendarwidgetdelegate.h"
#include "objects/eventfinder.h"
#include "objects/eventgateway.h"
#include "utility/utility.h"

CalendarWidgetDelegate::CalendarWidgetDelegate(QObject *parent)
	: QItemDelegate(parent)
{
	m_medal = QImage(":/images/medal");
	m_vote_high = QImage(":/images/vote_high");
	m_vote_low = QImage(":/images/vote_low");
}

void CalendarWidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QDate &date) const
{
	painter->save();

	EventGateway event;
	if (EventFinder::find(event, date)) {
		QFont font = painter->font();
		font.setBold(true);
		painter->setFont(font);

		QString type = event.eventType_description().toLower();
		QRect rect1 = painter->boundingRect(option.rect, Qt::AlignCenter | Qt::TextWordWrap, type);
		painter->drawText(option.rect, Qt::AlignCenter | Qt::TextWordWrap, type);

		font.setBold(false);
		painter->setFont(font);

		QString distance = Utility::formatDistance(event.distance(), 3);
		QRect rect2 = painter->boundingRect(option.rect, Qt::TextSingleLine, distance);
		rect2.moveCenter(option.rect.center());
		rect2.moveTop(rect1.bottom());
		painter->drawText(rect2, Qt::TextSingleLine, distance);

		int x = option.rect.right();

		if (event.eventType_hasMedal()) {
			x -= (m_medal.width() + 2);
			painter->drawImage(QPoint(x, option.rect.top() + 2), m_medal);
		}

		if (event.vote() >= 5) {
			x -= (m_vote_high.width() + 2);
			painter->drawImage(QPoint(x, option.rect.top() + 2), m_vote_high);
		}
		if (event.vote() <= 1) {
			x -= (m_vote_low.width() + 2);
			painter->drawImage(QPoint(x, option.rect.top() + 2), m_vote_low);
		}
	}

	painter->restore();
}

QString CalendarWidgetDelegate::toolTipText(const QDate &date) const
{
	QString tip = "";
	EventGateway event;
	if (EventFinder::find(event, date)) {
		tip += "<table border=0 cellpadding=0 cellspacing=6>";
		tip += "<tr><td colspan=2><i>" + event.start().date().toString(Qt::DefaultLocaleLongDate) + "</i></td></tr>";
		tip += "<tr><td>&nbsp;</td><td>&nbsp;</td></tr>";
		tip += "<tr><td>" + tr("Name:") + "</td><td><b>" + event.name() + "</b></td></tr>";
		tip += "<tr><td>" + tr("Description:") + "</td><td>" + event.description() + "</td></tr>";
		tip += "<tr><td>" + tr("Type:") + "</td><td>" + event.eventType_description() + "</td></tr>";
		tip += "<tr><td>" + tr("Start time:") + "</td><td>" + event.start().time().toString("hh:mm") + "</td></tr>";
		tip += "<tr><td>" + tr("Distance:") + "</td><td>" + Utility::formatDistance(event.distance(), 2) + "</td></tr>";
		tip += "<tr><td>" + tr("Duration:") + "</td><td>" + Utility::formatDuration(event.duration()) + "</td></tr>";
		tip += "</table>";
	}
	return tip;
}
