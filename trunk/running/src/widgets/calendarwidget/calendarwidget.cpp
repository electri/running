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
#include "calendarwidget.h"

CalendarWidget::CalendarWidget(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);

	QFont font;
	font.setBold(true);
	font.setPointSize(QApplication::font().pointSize() + 1);
	selectedDateLabel->setFont(font);

	connect(this, SIGNAL(selectedDateChanged(const QDate &)), calendarWidgetInternal, SLOT(setSelectedDate(const QDate &)));
	connect(calendarWidgetInternal, SIGNAL(selectedDateChanged(const QDate &)), this, SLOT(setSelectedDate(const QDate &)));
	connect(calendarWidgetInternal, SIGNAL(activated()), this, SIGNAL(activated()));

	m_selectedDate = QDate();
}

void CalendarWidget::setSelectedDate(const QDate &newDate)
{
	if (newDate.isValid()) {
		QDate oldDate = m_selectedDate;
		if (newDate != oldDate) {

			selectedDateLabel->setText(newDate.toString("d MMMM yyyy"));
			m_selectedDate = newDate;

			emit selectedDateChanged(newDate);
			emit selectionChanged();

			if((newDate.year() != oldDate.year()) && (newDate.month() != oldDate.month())) {
				emit currentPageChanged(newDate.year(), newDate.month());
			}
		}
	}
}

QDate CalendarWidget::selectedDate() const
{
	return m_selectedDate;
}

void CalendarWidget::setFirstDayOfWeek(const Qt::DayOfWeek &dayOfWeek)
{
	calendarWidgetInternal->setFirstDayOfWeek(dayOfWeek);
}

Qt::DayOfWeek CalendarWidget::firstDayOfWeek() const
{
	return calendarWidgetInternal->firstDayOfWeek();
}

void CalendarWidget::setDelegate(CalendarWidgetDelegate *delegate)
{
	calendarWidgetInternal->setDelegate(delegate);
}

CalendarWidgetDelegate *CalendarWidget::delegate() const
{
	return calendarWidgetInternal->delegate();
}

int CalendarWidget::monthShown() const
{
	return m_selectedDate.month();
}

int CalendarWidget::yearShown() const
{
	return m_selectedDate.year();
}

void CalendarWidget::on_prevMonthPushButton_clicked()
{
	setSelectedDate(m_selectedDate.addMonths(-1));
}

void CalendarWidget::on_nextMonthPushButton_clicked()
{
	setSelectedDate(m_selectedDate.addMonths(1));
}

void CalendarWidget::on_prevYearPushButton_clicked()
{
	setSelectedDate(m_selectedDate.addYears(-1));
}

void CalendarWidget::on_nextYearPushButton_clicked()
{
	setSelectedDate(m_selectedDate.addYears(1));
}
