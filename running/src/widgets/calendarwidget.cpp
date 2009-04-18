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
#include "ui_calendarwidget.h"

CalendarWidget::CalendarWidget(QWidget *parent)
	: QWidget(parent), ui(new Ui::CalendarWidget)
{
	ui->setupUi(this);

	connect(this, SIGNAL(selectedDateChanged(const QDate &)), ui->calendarWidgetInternal, SLOT(setSelectedDate(const QDate &)));
	connect(ui->calendarWidgetInternal, SIGNAL(selectedDateChanged(const QDate &)), this, SLOT(setSelectedDate(const QDate &)));
	connect(ui->calendarWidgetInternal, SIGNAL(activated()), this, SIGNAL(activated()));

	m_selectedDate = QDate();
}

CalendarWidget::~CalendarWidget()
{
	delete ui;
}

void CalendarWidget::setSelectedDate(const QDate &newDate)
{
	if (newDate.isValid()) {
		QDate oldDate = m_selectedDate;
		if (newDate != oldDate) {

			ui->selectedDateLabel->setText(newDate.toString("d MMMM yyyy"));
			m_selectedDate = newDate;

			emit selectedDateChanged(newDate);
			emit selectionChanged();

			if((newDate.year() != oldDate.year()) && (newDate.month() != oldDate.month())) {
				emit currentPageChanged(newDate.year(), newDate.month());
			}

//			qDebug() << "CalendarWidget::setSelectedDate(" + newDate.toString() + ")";
		}
	}
}

QDate CalendarWidget::selectedDate() const
{
	return m_selectedDate;
}

void CalendarWidget::setFirstDayOfWeek(const Qt::DayOfWeek &dayOfWeek)
{
	ui->calendarWidgetInternal->setFirstDayOfWeek(dayOfWeek);
}

Qt::DayOfWeek CalendarWidget::firstDayOfWeek() const
{
	return ui->calendarWidgetInternal->firstDayOfWeek();
}

void CalendarWidget::setDelegate(CalendarDelegate *delegate)
{
	ui->calendarWidgetInternal->setDelegate(delegate);
}

CalendarDelegate *CalendarWidget::delegate() const
{
	return 	ui->calendarWidgetInternal->delegate();
}

int CalendarWidget::monthShown () const
{
	return m_selectedDate.month();
}

int CalendarWidget::yearShown () const
{
	return m_selectedDate.year();
}

void CalendarWidget::on_prevMonthToolButton_clicked()
{
	setSelectedDate(m_selectedDate.addMonths(-1));
}

void CalendarWidget::on_nextMonthToolButton_clicked()
{
	setSelectedDate(m_selectedDate.addMonths(1));
}

void CalendarWidget::on_prevYearToolButton_clicked()
{
	setSelectedDate(m_selectedDate.addYears(-1));
}

void CalendarWidget::on_nextYearToolButton_clicked()
{
	setSelectedDate(m_selectedDate.addYears(1));
}
