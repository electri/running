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

#ifndef CALENDARDELEGATE_H
#define CALENDARDELEGATE_H

#include <QItemDelegate>

class QPainter;
namespace Objects { class Event; }

class CalendarDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	CalendarDelegate(QObject *parent = 0);

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QDate &date) const;
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QDate &date, Objects::Event *event) const;

private:
	QImage m_medal;
	QImage m_vote_high, m_vote_low;
};

#endif
