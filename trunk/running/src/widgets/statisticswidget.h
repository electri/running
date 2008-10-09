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

#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include <QPainter>

#include "../utility/statisticsservice.h"

namespace StatisticsPages {
	enum Page { EventsPerDate, Shoes };
}

class StatisticsWidget : public QWidget
{
	Q_OBJECT

public:
	StatisticsWidget(QWidget *parent = 0);

	void setPage(StatisticsPages::Page);

//	void resetHeight();

private:
	StatisticsPages::Page m_page;

	void paintEvent(QPaintEvent *);
	void drawHeader(QPainter &painter, int x, int y, int cw, int ch);
	void drawRow(QPainter &painter, int x, int y, int cw, int ch, int col, const StatisticsResults::EventsPerDate &row);
	void drawRow(QPainter &painter, int x, int y, int cw, int ch, int col, const StatisticsResults::Shoes &row);

	int findMinimumHeight();

	int margin, cellheight, cellwidth;
};

#endif
