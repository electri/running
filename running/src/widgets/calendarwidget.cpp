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
#include "../objects/event.h"
#include "../services/objectmap.h"

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

	Services::ObjectMap *session = Services::ObjectMap::instance();
	QList<Objects::BaseObject *> list = session->getEventsByDate(date, date);
	if (list.count() > 0) {
		Objects::Event *event = static_cast<Objects::Event *>(list.at(0));
		m_delegate->paint(painter, option, date, event);
	}
	else {
		m_delegate->paint(painter, option, date);
	}
	QList<Objects::BaseObject *>::const_iterator i = list.constBegin();
	while (i != list.constEnd()) {
		session->discardObject(*i);
		++i;
	}
}
