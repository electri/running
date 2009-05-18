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

#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QWidget>
#include <QDate>
#include "ui_calendarwidget.h"

class CalendarWidgetDelegate;

class CalendarWidget : public QWidget, Ui::CalendarWidget
{
	Q_OBJECT

	Q_PROPERTY(QDate selectedDate READ selectedDate WRITE setSelectedDate)
	Q_PROPERTY(Qt::DayOfWeek firstDayOfWeek READ firstDayOfWeek WRITE setFirstDayOfWeek)

public:
	CalendarWidget(QWidget *parent = 0);

	QDate selectedDate() const;
	Qt::DayOfWeek firstDayOfWeek() const;
	CalendarWidgetDelegate *delegate() const;
	int monthShown() const;
	int yearShown() const;

signals:
	void selectedDateChanged(const QDate &newDate);
	void selectionChanged();
	void currentPageChanged(int year, int month);
	void activated();

public slots:
	void setSelectedDate(const QDate &date);
	void setFirstDayOfWeek(const Qt::DayOfWeek &dayOfWeek);
	void setDelegate(CalendarWidgetDelegate *delegate);
	void on_prevMonthPushButton_clicked();
	void on_nextMonthPushButton_clicked();
	void on_prevYearPushButton_clicked();
	void on_nextYearPushButton_clicked();

private:
	QDate m_selectedDate;
};

#endif // CALENDARWIDGET_H
