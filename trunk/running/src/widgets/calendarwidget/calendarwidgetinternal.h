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

#ifndef CALENDARWIDGETINTERNAL_H
#define CALENDARWIDGETINTERNAL_H

#include <QWidget>
#include <QDate>

class CalendarDelegate;

class CalendarWidgetInternal : public QWidget
{
	Q_OBJECT

	Q_PROPERTY(QDate selectedDate READ selectedDate WRITE setSelectedDate)
	Q_PROPERTY(Qt::DayOfWeek firstDayOfWeek READ firstDayOfWeek WRITE setFirstDayOfWeek)

public:
	CalendarWidgetInternal(QWidget *parent = 0);

	QDate selectedDate() const;
	Qt::DayOfWeek firstDayOfWeek() const;
	CalendarDelegate *delegate() const;

signals:
	void selectedDateChanged(const QDate &newDate);
	void activated();

public slots:
	void setSelectedDate(const QDate &date);
	void setFirstDayOfWeek(const Qt::DayOfWeek &dayOfWeek);
	void setDelegate(CalendarDelegate *delegate);

protected:
	bool event(QEvent *event);
	void paintEvent(QPaintEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

private:
	void drawHeaderCell(QPainter *painter, const QRect &rect, int weekday);
	void drawEmptyCell(QPainter *painter, const QRect &rect);
	void drawItemCell(QPainter *painter, const QRect &rect, int weekday, int day);
	void computeMonthBoundaries();
	QDate findDate(QPoint pos) const;
	bool isDate(QPoint pos) const;

	QDate m_selectedDate;
	Qt::DayOfWeek m_firstDayOfWeek;
	CalendarDelegate *m_delegate;

	quint8 m_monthStartAt, m_monthEndAt;
	int m_headHeight, m_cellHeight, m_cellWidth;
};

#endif // CALENDARWIDGETINTERNAL_H
