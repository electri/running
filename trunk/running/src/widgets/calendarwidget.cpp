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

#include "calendarwidget.h"

#include "../delegates/calendardelegate.h"

#include "../application.h"
#include "../objects/event.h"

CalendarWidget::CalendarWidget(QWidget *parent)
	: QCalendarWidget(parent)
{
	m_delegate = new CalendarDelegate(this);

//	QTableView *table = this->findChild<QTableView *>();
//	connect(table, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(tableDoubleClicked(const QModelIndex &)));
}

void CalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
	QStyleOptionViewItem option;
	option.initFrom(this);
	option.rect = rect;
	option.showDecorationSelected = true;
	if (date == selectedDate()) {
		option.state |= QStyle::State_Selected | QStyle::State_HasFocus;
	} else {
		option.state &= ~QStyle::State_HasFocus;
	}

	if (date.month() != monthShown()) {
		option.palette.setColor(QPalette::Text, QColor("gray"));
	}
	else {
		int dayOfWeek = date.dayOfWeek();
		if (dayOfWeek == 6 || dayOfWeek == 7) {
			option.palette.setColor(QPalette::Text, QColor("red"));
		}
	}

	Objects::Event *currentEvent = NULL;
    Services::ObjectMap *session = APP->objectMap();

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
		m_delegate->paint(painter, option, date, currentEvent);
		session->discardObject(currentEvent);
	}
	else {
		m_delegate->paint(painter, option, date);
	}
}
