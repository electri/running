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
#include "statisticswidget.h"
//#include "utility.h"

StatisticsWidget::StatisticsWidget(QWidget *parent)
	: QWidget(parent)
{
//	m_page = StatisticsPages::EventsPerDate;
//
//	margin = 5;
//	cellwidth = 130;
//	cellheight = 28;
}

//void StatisticsWidget::setPage(StatisticsPages::Page page)
//{
//	m_page = page;
//
//	this->setMinimumHeight(this->findMinimumHeight());
//	this->update();
//}

//void StatisticsWidget::drawHeader(QPainter &painter, int x, int y, int cw, int ch)
//{
//	QPen pen1(palette().text().color());
//	QPen pen2(palette().shadow().color());
//	int flags = Qt::AlignCenter;
//
//	painter.setPen(pen1);
//	painter.drawText(x + cw, y, cw, ch, flags, tr("DISTANCE\n(km)"));
//	painter.drawText(x + (cw * 2), y, cw, ch, flags, tr("DURATION\n(h min sec)"));
//	switch (m_page)
//	{
//		case StatisticsPages::EventsPerDate:
//			painter.drawText(x + (cw * 3), y, cw, ch, flags, tr("AVR. SPEED\n(min/km - km/h)"));
//			break;
//		case StatisticsPages::Shoes:
//			painter.drawText(x + (cw * 3), y, cw, ch, flags, tr("COST\n(/km)"));
//			break;
//	}
//
//	pen2.setWidth(4);
//	painter.setPen(pen2);
//	painter.drawLine(x, y + ch, x + (cw * 4) - 1, y + ch);
//}

//void StatisticsWidget::drawRow(QPainter &painter, int x, int y, int cw, int ch, int col, const StatisticsResults::EventsPerDate &row)
//{
//	QPen pen1(palette().text().color());
//	QPen pen2(palette().shadow().color());
//	int flags = Qt::AlignCenter;
//
//	if (col % 2) {
//		painter.fillRect(x + cw, y, cw * 3, ch, palette().midlight());
//	}
//
//	QString field1, field2, field3, field4;
//	if (row.month != 0) {
//		field1 = QString("%1 %2").arg(QDate::longMonthName(row.month)).arg(row.year);
//	} else if (row.year != 0) {
//		field1 = QString("%1").arg(row.year);
//	} else {
//		field1 = tr("TOTALS");
//	}
//	field2 = Utility::formatDistance(row.distance, 3);
//	field3 = Utility::formatDuration(row.duration);
//	field4 = tr("%1 / %2").arg(Utility::formatDuration(row.paceTime)).arg(Utility::formatDistance(row.paceSpeed, 2));
//
//	painter.setPen(pen1);
//	painter.drawText(x, y, cw, ch, flags, field1);
//	painter.drawText(x + cw, y, cw, ch, flags, field2);
//	painter.drawText(x + (cw * 2), y, cw, ch, flags, field3);
//	painter.drawText(x + (cw * 3), y, cw, ch, flags, field4);
//
//	painter.setPen(pen2);
//	painter.drawLine(x + cw, y, x + cw, y + ch);
//	painter.drawLine(x + (cw * 4), y, x + (cw * 4), y + ch);
//}

//void StatisticsWidget::drawRow(QPainter &painter, int x, int y, int cw, int ch, int col, const StatisticsResults::Shoes &row)
//{
//	QPen pen1(palette().text().color());
//	QPen pen2(palette().shadow().color());
//	int flags = Qt::AlignCenter;
//
//	if (col % 2) {
//		painter.fillRect(x + cw, y, cw * 3, ch, palette().midlight());
//	}
//
//	QString field1, field2, field3, field4;
//	field1 = QString("%1").arg(row.description);
//	field2 = Utility::formatDistance(row.distance, 3);
//	field3 = Utility::formatDuration(row.duration);
//	field4 = Utility::formatDistance(row.pricePerDistance, 2);
//
//	painter.setPen(pen1);
//	painter.drawText(x, y, cw, ch, flags, field1);
//	painter.drawText(x + cw, y, cw, ch, flags, field2);
//	painter.drawText(x + (cw * 2), y, cw, ch, flags, field3);
//	painter.drawText(x + (cw * 3), y, cw, ch, flags, field4);
//
//	painter.setPen(pen2);
//	painter.drawLine(x + cw, y, x + cw, y + ch);
//	painter.drawLine(x + (cw * 4), y, x + (cw * 4), y + ch);
//}

void StatisticsWidget::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

//	QPainter painter(this);
//
//	painter.setFont(QFont("MS Shell Dlg 2", 9));
//
//	QPalette palette = this->palette();
//	switch (m_page)
//	{
//		case StatisticsPages::EventsPerDate:
//			palette.setColor(QPalette::Shadow, QColor(0, 128, 0));
//			palette.setColor(QPalette::Midlight, QColor(192, 255, 192));
//			break;
//		case StatisticsPages::Shoes:
//			palette.setColor(QPalette::Shadow, QColor(128, 0, 0));
//			palette.setColor(QPalette::Midlight, QColor(255, 192, 192));
//			break;
//	}
//	this->setPalette(palette);
//
//	cellwidth = (rect().width() - (margin * 2)) / 4;
//
//	int x = margin;
//	int y = margin;
//	int col = 1;
//
//	this->drawHeader(painter, x, y, cellwidth, cellheight + 16);
//	y += (cellheight + 16 + 2);
//
//	switch (m_page)
//	{
//		case StatisticsPages::EventsPerDate:
//		{
//			QList<StatisticsResults::EventsPerDate> list = Utility::StatisticsService::instance()->eventsPerDate();
//			QList<StatisticsResults::EventsPerDate>::const_iterator it = list.constBegin();
//			while (it != list.constEnd()) {
//				this->drawRow(painter, x, y, cellwidth, cellheight, col, *it++);
//				y += cellheight;
//				col++;
//			}
//			//painter.drawLine(x, y, x + (cellwidth * 4), y);
//			//y++;
//		}
//		break;
//
//		case StatisticsPages::Shoes:
//		{
//			QList<StatisticsResults::Shoes> list = Utility::StatisticsService::instance()->shoes();
//			QList<StatisticsResults::Shoes>::const_iterator it = list.constBegin();
//			while (it != list.constEnd()) {
//				this->drawRow(painter, x, y, cellwidth, cellheight, col, *it++);
//				y += cellheight;
//				col++;
//			}
//		}
//		break;
//	}
}

//int StatisticsWidget::findMinimumHeight()
//{
//	int height = 0;
//	height += margin;
//	height += (cellheight + 16 + 2); // header
//	switch (m_page)
//	{
//		case StatisticsPages::EventsPerDate:
//		{
//			QList<StatisticsResults::EventsPerDate> list = Utility::StatisticsService::instance()->eventsPerDate();
//			height += (cellheight * list.size());
//		}
//		break;
//		case StatisticsPages::Shoes:
//		{
//			QList<StatisticsResults::Shoes> list = Utility::StatisticsService::instance()->shoes();
//			height += (cellheight * list.size());
//		}
//		break;
//	}
//	height += margin;
//
//	return height;
//}
