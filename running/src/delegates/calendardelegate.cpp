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

#include "calendardelegate.h"
#include "../objects/event.h"
#include "../objects/eventtype.h"
#include "../application.h"
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
//	drawBackground(painter, option, QModelIndex());
//	drawFocus(painter, option, option.rect);

	Objects::Event *currentEvent = NULL;
	Services::ObjectMap *session = Application::instance()->objectMap();

	QList<Objects::BaseObject *> list = session->getEventsByDate(date, date);
	if (!list.isEmpty()) {
		currentEvent = static_cast<Objects::Event *>(list.takeFirst());
	}
	if (!list.isEmpty()) {
		foreach (Objects::BaseObject *object, list) {
			session->discardObject(object);
		}
	}

	if (currentEvent) {
		painter->save();

		QFont font = painter->font();
		font.setBold(true);
		painter->setFont(font);

		QString type = currentEvent->eventType()->description();
		type = type.toLower();
		QRect rect1 = painter->boundingRect(option.rect, Qt::AlignCenter | Qt::TextWordWrap, type);
		painter->drawText(option.rect, Qt::AlignCenter | Qt::TextWordWrap, type);

		font.setBold(false);
		painter->setFont(font);

		QString distance = Utility::formatDistance(currentEvent->distance(), 3) + " km";
		QRect rect2 = painter->boundingRect(option.rect, Qt::TextSingleLine, distance);
		rect2.moveCenter(option.rect.center());
		rect2.moveTop(rect1.bottom());
		painter->drawText(rect2, Qt::TextSingleLine, distance);

		int x = option.rect.right();
		if (currentEvent->eventType()->hasMedal()) {
			x -= (m_medal.width() + 2);
			painter->drawImage(QPoint(x, option.rect.top() + 2), m_medal);
		}
		if (currentEvent->vote() >= 5) {
			x -= (m_vote_high.width() + 2);
			painter->drawImage(QPoint(x, option.rect.top() + 2), m_vote_high);
		}
		if (currentEvent->vote() <= 1) {
			x -= (m_vote_low.width() + 2);
			painter->drawImage(QPoint(x, option.rect.top() + 2), m_vote_low);
		}

		painter->restore();

		session->discardObject(currentEvent);
	}
}

QString CalendarDelegate::toolTipText(const QDate &date) const
{
	Objects::Event *currentEvent = NULL;
	Services::ObjectMap *session = Application::instance()->objectMap();

	QList<Objects::BaseObject *> list = session->getEventsByDate(date, date);
	if (!list.isEmpty()) {
		currentEvent = static_cast<Objects::Event *>(list.takeFirst());
	}
	if (!list.isEmpty()) {
		foreach (Objects::BaseObject *object, list) {
			session->discardObject(object);
		}
	}

	QString tip = "";
	if (currentEvent) {
		tip += "<table border=0 cellpadding=0 cellspacing=6>";
		tip += "<tr><td colspan=2><i>" + currentEvent->start().date().toString(Qt::DefaultLocaleLongDate) + "</i></td></tr>";
		tip += "<tr><td>&nbsp;</td><td>&nbsp;</td></tr>";
		tip += "<tr><td>" + tr("Nome:") + "</td><td><b>" + currentEvent->name() + "</b></td></tr>";
		tip += "<tr><td>" + tr("Descrizione:") + "</td><td>" + currentEvent->description() + "</td></tr>";
		tip += "<tr><td>" + tr("Tipo:") + "</td><td>" + currentEvent->eventType()->description() + "</td></tr>";
		tip += "<tr><td>" + tr("Ora di inizio:") + "</td><td>" + currentEvent->start().time().toString("hh:mm") + "</td></tr>";
		tip += "<tr><td>" + tr("Distanza:") + "</td><td>" + Utility::formatDistance(currentEvent->distance(), 2) + " " + Application::instance()->cfg()->cfgDistanceUnit()->description() + "</td></tr>";
		tip += "<tr><td>" + tr("Durata:") + "</td><td>" + Utility::formatDuration(currentEvent->duration()) + "</td></tr>";
		tip += "</table>";

		session->discardObject(currentEvent);
	}
	return tip;
}
