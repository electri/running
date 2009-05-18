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
#include "calendarwidgetinternal.h"
#include "calendarwidgetdelegate.h"

CalendarWidgetInternal::CalendarWidgetInternal(QWidget *parent)
	: QWidget(parent)
{
	setAutoFillBackground(true);
	setBackgroundRole(QPalette::AlternateBase);
	setFocusPolicy(Qt::WheelFocus);

	m_selectedDate = QDate();
	m_firstDayOfWeek = Qt::Monday;
	m_delegate = NULL;
	m_monthStartAt = m_monthEndAt = 0;
	m_headHeight = m_cellHeight = m_cellWidth = 0;
}

void CalendarWidgetInternal::setSelectedDate(const QDate &date)
{
	if (date.isValid()) {
		if (date != m_selectedDate) {
			m_selectedDate = date;
			computeMonthBoundaries();
			update();

			emit selectedDateChanged(date);
		}
	}
}

QDate CalendarWidgetInternal::selectedDate() const
{
	return m_selectedDate;
}

void CalendarWidgetInternal::setDelegate(CalendarWidgetDelegate *delegate)
{
	m_delegate = delegate;
}

CalendarWidgetDelegate *CalendarWidgetInternal::delegate() const
{
	return m_delegate;
}

void CalendarWidgetInternal::setFirstDayOfWeek(const Qt::DayOfWeek &dayOfWeek)
{
	m_firstDayOfWeek = dayOfWeek;
	computeMonthBoundaries();
	update();
}

Qt::DayOfWeek CalendarWidgetInternal::firstDayOfWeek() const
{
	return m_firstDayOfWeek;
}

bool CalendarWidgetInternal::event(QEvent *event)
{
	if (event->type() == QEvent::ToolTip) {
		QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);

		if (isDate(helpEvent->pos())) {
			if (m_delegate) {
				QString tip = m_delegate->toolTipText(findDate(helpEvent->pos()));
				QToolTip::showText(helpEvent->globalPos(), tip);
				return true;
			}
		}

		QToolTip::hideText();
	}

	return QWidget::event(event);
}

void CalendarWidgetInternal::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setPen(palette().color(QPalette::Active, QPalette::Dark));

	m_headHeight = 24;
	m_cellHeight = (height() - m_headHeight) / 6;
	m_cellWidth = width() / 7;

	for (int y = 0; y < 7; ++y) {
		int weekday = y + m_firstDayOfWeek;
		if (weekday > 7) weekday -= 7;

		QRect rect((m_cellWidth * y), 0, y != 6 ? m_cellWidth : width() - (m_cellWidth * y) - 1, m_headHeight);
		drawHeaderCell(&painter, rect, weekday);

		for (int x = 0; x < 6; ++x) {
			QRect rect((m_cellWidth * y), m_headHeight + (m_cellHeight * x), y != 6 ? m_cellWidth : width() - (m_cellWidth * y) - 1, x != 5 ? m_cellHeight : height() - (m_headHeight + (m_cellHeight * x)) - 1);

			int offset = ((x * 7) + y) + 1;
			if ((offset >= m_monthStartAt) && (offset <= m_monthEndAt)) {
				int day = offset - m_monthStartAt + 1;
				drawItemCell(&painter, rect, weekday, day);
			} else {
				drawEmptyCell(&painter, rect);
			}
		}
	}
}

void CalendarWidgetInternal::drawHeaderCell(QPainter *painter, const QRect &rect, int weekday)
{
	painter->save();

	painter->drawRect(rect);

	QString dayName = QLocale::system().dayName(weekday);
	painter->setPen(((weekday == Qt::Saturday) || (weekday == Qt::Sunday)) ? QColor(Qt::red) : palette().color(QPalette::Active, QPalette::Text));
	painter->drawText(rect, Qt::AlignCenter | Qt::TextSingleLine, dayName);

	painter->restore();
}

void CalendarWidgetInternal::drawEmptyCell(QPainter *painter, const QRect &rect)
{
	painter->drawRect(rect);
}

void CalendarWidgetInternal::drawItemCell(QPainter *painter, const QRect &rect, int weekday, int day)
{
	painter->save();

	if (day == m_selectedDate.day()) {
		painter->setPen(palette().color(QPalette::Active, QPalette::Dark));
		painter->setBrush(palette().brush(hasFocus() ? QPalette::Active : QPalette::Inactive, QPalette::Highlight));
		painter->drawRect(rect);

		painter->setPen(palette().color(hasFocus() ? QPalette::Active : QPalette::Inactive, QPalette::HighlightedText));
		painter->drawText((rect.topLeft() + QPoint(4, 12)), QString("%1").arg(day));
	} else {
		painter->setPen(palette().color(QPalette::Active, QPalette::Dark));
		painter->setBrush(palette().brush(QPalette::Base));
		painter->drawRect(rect);

		painter->setPen(((weekday == Qt::Saturday) || (weekday == Qt::Sunday)) ? QColor(Qt::red) : palette().color(QPalette::Active, QPalette::Text));
		painter->drawText((rect.topLeft() + QPoint(4, 12)), QString("%1").arg(day));
	}

	if (m_delegate) {
		QDate date = QDate(m_selectedDate.year(), m_selectedDate.month(), day);

		QStyleOptionViewItem option;
		option.initFrom(this);
		option.rect = rect;
		option.showDecorationSelected = true;
		if (date == selectedDate()) {
			option.state |= QStyle::State_Selected | QStyle::State_HasFocus;
		} else {
			option.state &= ~QStyle::State_HasFocus;
		}

		m_delegate->paint(painter, option, date);
	}

	painter->restore();
}

void CalendarWidgetInternal::computeMonthBoundaries()
{
	/* m_firstDayOfWeek is enum Qt::DayOfWeek and eval to 1-7
	 * the month start at first cell if the first day is monday and the
	 * first column is set to monday too, and so on*/

	int dayOfWeek = QDate(m_selectedDate.year(), m_selectedDate.month(), 1).dayOfWeek();
	if (dayOfWeek < m_firstDayOfWeek) {
		m_monthStartAt = dayOfWeek + (8 - m_firstDayOfWeek);
	} else if (dayOfWeek > m_firstDayOfWeek) {
		m_monthStartAt = dayOfWeek + (m_firstDayOfWeek - 1);
	} else {
		m_monthStartAt = 1;
	}
	m_monthEndAt = m_monthStartAt + m_selectedDate.daysInMonth() - 1;
}

QDate CalendarWidgetInternal::findDate(QPoint pos) const
{
	if (!isDate(pos)) return QDate();

	int columnID = pos.x() / m_cellWidth;
	int rowID = (pos.y() - m_headHeight) / m_cellHeight;

	int offset = ((rowID * 7) + columnID) + 1;
	int day = offset - m_monthStartAt + 1;

	return QDate(m_selectedDate.year(), m_selectedDate.month(), day);
}

bool CalendarWidgetInternal::isDate(QPoint pos) const
{
	QRect rect(0, m_headHeight + 1, width(), height());
	return rect.contains(pos);
}

void CalendarWidgetInternal::keyPressEvent(QKeyEvent *event)
{
	int key = event->key();
	switch (key) {
		case Qt::Key_Left:
		case Qt::Key_Right:
		{
			int offset = ((key == Qt::Key_Left) ? -1 : 1);
			Qt::KeyboardModifiers modifiers = event->modifiers();
			switch (modifiers) {
				case Qt::ControlModifier:
					setSelectedDate(m_selectedDate.addYears(offset));
					break;
				case Qt::ShiftModifier:
					setSelectedDate(m_selectedDate.addMonths(offset));
					break;
				default:
					setSelectedDate(m_selectedDate.addDays(offset));
					break;
			}
			break;
		}
		case Qt::Key_Up:
		case Qt::Key_Down:
		{
			int offset = ((key == Qt::Key_Up) ? -7 : 7);
			setSelectedDate(m_selectedDate.addDays(offset));
			break;
		}
		case Qt::Key_Return:
		case Qt::Key_Enter:
			emit activated();
			break;
		default:
			QWidget::keyPressEvent(event);
	}
}

void CalendarWidgetInternal::mousePressEvent(QMouseEvent *event)
{
	if (isDate(event->pos())) {
		setSelectedDate(findDate(event->pos()));
	}
}

void CalendarWidgetInternal::mouseReleaseEvent(QMouseEvent *event)
{
	Q_UNUSED(event);
}

void CalendarWidgetInternal::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (isDate(event->pos())) {
		emit activated();
	}
}

void CalendarWidgetInternal::wheelEvent(QWheelEvent *event)
{
	int offset = event->delta() > 0 ? -1 : 1;
	if (offset != 0) {
		switch (event->modifiers()) {
			case Qt::ControlModifier:
				setSelectedDate(m_selectedDate.addYears(offset));
				break;
			case Qt::ShiftModifier:
				setSelectedDate(m_selectedDate.addMonths(offset));
				break;
			case Qt::NoModifier:
				setSelectedDate(m_selectedDate.addDays(offset));
				break;
		}
	}
}
