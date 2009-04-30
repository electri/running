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
#include <QtSql>
#include "statisticswidget.h"
#include "objects/settingsgateway.h"
#include "utility/utility.h"

struct EventsPerDate
{
	int year, month;
	int events;
	double distance;
	QDateTime duration;
	QTime paceTime;
	double paceSpeed;

	EventsPerDate()
	{
		year = month = 0;
		events = 0;
		distance = 0.0;
		duration = QDateTime(QDate(1970, 1, 1));
		paceTime = QTime();
		paceSpeed = 0.0;
	}
};
static QList<EventsPerDate> sm_eventsPerDate;

struct Shoes
{
	int id;
	QString description;
	int events;
	double distance;
	QDateTime duration;
	double initialDistance;
	double price;
	bool retired;
	double pricePerDistance;

	Shoes()
	{
		id = 0;
		description = QString();
		events = 0;
		distance = 0.0;
		duration = QDateTime(QDate(1970, 1, 1));
		initialDistance = 0.0;
		price = 0.0;
		retired = false;
		pricePerDistance = 0.0;
	}
};
static QList<Shoes> sm_shoes;

StatisticsWidget::StatisticsWidget(QWidget *parent)
	: QWidget(parent)
{
	m_page = StatisticsPageEventsPerDate;
	m_margin = 5;
	m_cellwidth = 130;
	m_cellheight = 28;
}

void StatisticsWidget::setPage(StatisticsPage page)
{
	m_page = page;

	setMinimumHeight(_minimumHeight());
	update();
}

void StatisticsWidget::refreshCache()
{
	sm_eventsPerDate.clear();
	sm_shoes.clear();

	QString queryText;
	QSqlDatabase db = QSqlDatabase::database();
	if (db.isValid()) {
		if (db.isOpen()) {
			QSqlQuery query(db);

			queryText = QString("SELECT 0, 0, count(Id), sum(Distance), datetime('1970-01-01',sum(strftime('%s','1970-01-01T' || Duration)) || ' seconds') FROM Event");
			query.exec(queryText);
			while (query.next()) {
				_cacheEventsPerDateRow(query.record());
			}

			queryText = QString("SELECT strftime('%Y',Start), 0, count(Id), sum(Distance), datetime('1970-01-01',sum(strftime('%s','1970-01-01T' || Duration)) || ' seconds') FROM Event GROUP BY strftime('%Y',start) ORDER BY strftime('%Y',start)");
			query.exec(queryText);
			while (query.next()) {
				_cacheEventsPerDateRow(query.record());
			}

			queryText = QString("SELECT strftime('%Y',Start), strftime('%m',start), count(Id), sum(Distance), datetime('1970-01-01',sum(strftime('%s','1970-01-01T' || Duration)) || ' seconds') FROM Event GROUP BY strftime('%Y%m',start) ORDER BY strftime('%Y%m',start)");
			query.exec(queryText);
			while (query.next()) {
				_cacheEventsPerDateRow(query.record());
			}

			queryText = QString("SELECT ShoeView.Id, ShoeView.Description, count(Event.Id), sum(Event.Distance), datetime('1970-01-01',sum(strftime('%s','1970-01-01T' || Event.Duration)) || ' seconds'), ShoeView.InitialDistance, ShoeView.Price, ShoeView.Retired FROM ShoeView LEFT JOIN Event ON Event.ShoeId = ShoeView.Id GROUP BY ShoeView.Id ORDER BY ShoeView.Id");
			query.exec(queryText);
			while (query.next()) {
				_cacheShoesRow(query.record());
			}
		}
	}

	setMinimumHeight(_minimumHeight());
}

void StatisticsWidget::_cacheEventsPerDateRow(const QSqlRecord &record)
{
	if (!record.isEmpty()) {
		EventsPerDate r;
		r.year = record.value(0).toInt();
		r.month = record.value(1).toInt();
		r.events = record.value(2).toInt();
		r.distance = record.value(3).toDouble();
		r.duration = record.value(4).toDateTime();
		if (r.distance > 0) {
			r.paceTime = Utility::paceTime(r.distance, r.duration);
			r.paceSpeed = Utility::paceSpeed(r.distance, r.duration);
		}
		sm_eventsPerDate.append(r);
	}
}

void StatisticsWidget::_cacheShoesRow(const QSqlRecord &record)
{
	if (!record.isEmpty()) {
		Shoes r;
		r.id = record.value(0).toInt();
		r.description = record.value(1).toString();
		r.events = record.value(2).toInt();
		r.distance = record.value(3).toDouble();
		r.duration = record.value(4).toDateTime();
		r.initialDistance = record.value(5).toDouble();
		r.price = record.value(6).toDouble();
		r.retired = record.value(7).toBool();
		double distance = r.distance + r.initialDistance;
		if (distance > 0) {
			r.pricePerDistance = r.price / distance;
		}
		sm_shoes.append(r);
	}
}

void StatisticsWidget::_drawHeader(QPainter &painter, int x, int y, int cw, int ch)
{
	QPen pen1(palette().text().color());
	QPen pen2(palette().shadow().color());
	int flags = Qt::AlignCenter;

	painter.setPen(pen1);
	painter.drawText(x + cw, y, cw, ch, flags, tr("DISTANCE\n(%1)").arg(SettingsGateway::instance()->distanceUnit_description()));
	painter.drawText(x + (cw * 2), y, cw, ch, flags, tr("DURATION\n(h min sec)"));
	switch (m_page)
	{
		case StatisticsPageEventsPerDate:
			painter.drawText(x + (cw * 3), y, cw, ch, flags, tr("AVR. SPEED\n(min/%1 - %1/h)").arg(SettingsGateway::instance()->distanceUnit_description()));
			break;
		case StatisticsPageShoes:
			painter.drawText(x + (cw * 3), y, cw, ch, flags, tr("COST\n(%1/%2)").arg(SettingsGateway::instance()->currencyUnit_description()).arg(SettingsGateway::instance()->distanceUnit_description()));
			break;
	}

	pen2.setWidth(4);
	painter.setPen(pen2);
	painter.drawLine(x, y + ch, x + (cw * 4) - 1, y + ch);
}

void StatisticsWidget::_drawEventsPerDateRow(QPainter &painter, int x, int y, int cw, int ch, int row)
{
	QPen pen1(palette().text().color());
	QPen pen2(palette().shadow().color());
	int flags = Qt::AlignCenter;

	if (row % 2) {
		painter.fillRect(x + cw, y, cw * 3, ch, palette().midlight());
	}

	EventsPerDate r = sm_eventsPerDate[row];

	QString field1, field2, field3, field4;
	if (r.month != 0) {
		field1 = QString("%1 %2").arg(QDate::longMonthName(r.month)).arg(r.year);
	} else if (r.year != 0) {
		field1 = QString("%1").arg(r.year);
	} else {
		field1 = tr("TOTALS");
	}
	field2 = Utility::formatReal(r.distance, 3);
	field3 = Utility::formatTime(r.duration);
	field4 = tr("%1 / %2").arg(Utility::formatTime(r.paceTime))
						  .arg(Utility::formatReal(r.paceSpeed, 2));

	painter.setPen(pen1);
	painter.drawText(x, y, cw, ch, flags, field1);
	painter.drawText(x + cw, y, cw, ch, flags, field2);
	painter.drawText(x + (cw * 2), y, cw, ch, flags, field3);
	painter.drawText(x + (cw * 3), y, cw, ch, flags, field4);

	painter.setPen(pen2);
	painter.drawLine(x + cw, y, x + cw, y + ch);
	painter.drawLine(x + (cw * 4), y, x + (cw * 4), y + ch);
}

void StatisticsWidget::_drawShoesRow(QPainter &painter, int x, int y, int cw, int ch, int row)
{
	QPen pen1(palette().text().color());
	QPen pen2(palette().shadow().color());
	int flags = Qt::AlignCenter;

	if (row % 2) {
		painter.fillRect(x + cw, y, cw * 3, ch, palette().midlight());
	}

	Shoes r = sm_shoes[row];

	QString field1, field2, field3, field4;
	field1 = QString("%1").arg(r.description);
	field2 = Utility::formatReal(r.distance, 3);
	field3 = Utility::formatTime(r.duration);
	field4 = Utility::formatReal(r.pricePerDistance, 2);

	painter.setPen(pen1);
	painter.drawText(x, y, cw, ch, flags, field1);
	painter.drawText(x + cw, y, cw, ch, flags, field2);
	painter.drawText(x + (cw * 2), y, cw, ch, flags, field3);
	painter.drawText(x + (cw * 3), y, cw, ch, flags, field4);

	painter.setPen(pen2);
	painter.drawLine(x + cw, y, x + cw, y + ch);
	painter.drawLine(x + (cw * 4), y, x + (cw * 4), y + ch);
}

void StatisticsWidget::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	painter.setFont(QFont("Arial", 9));

	QPalette palette = this->palette();
	switch (m_page)
	{
		case StatisticsPageEventsPerDate:
			palette.setColor(QPalette::Shadow, QColor(0, 128, 0));
			palette.setColor(QPalette::Midlight, QColor(192, 255, 192));
			break;
		case StatisticsPageShoes:
			palette.setColor(QPalette::Shadow, QColor(128, 0, 0));
			palette.setColor(QPalette::Midlight, QColor(255, 192, 192));
			break;
	}
	setPalette(palette);

	m_cellwidth = (rect().width() - (m_margin * 2)) / 4;

	int x = m_margin;
	int y = m_margin;

	_drawHeader(painter, x, y, m_cellwidth, m_cellheight + 16);
	y += (m_cellheight + 16 + 2);

	switch (m_page) {
		case StatisticsPageEventsPerDate:
			for (int row = 0; row < sm_eventsPerDate.count(); ++row) {
				_drawEventsPerDateRow(painter, x, y, m_cellwidth, m_cellheight, row);
				y += m_cellheight;
			}
//			painter.drawLine(x, y, x + (m_cellwidth * 4), y);
//			y++;
			break;
		case StatisticsPageShoes:
			for (int row = 0; row < sm_shoes.count(); ++row) {
				_drawShoesRow(painter, x, y, m_cellwidth, m_cellheight, row);
				y += m_cellheight;
			}
			break;
	}
}

int StatisticsWidget::_minimumHeight()
{
	int h = 0;
	h += m_margin;
	h += (m_cellheight + 16 + 2); // header
	switch (m_page) {
		case StatisticsPageEventsPerDate:
			h += (m_cellheight * sm_eventsPerDate.count());
			break;
		case StatisticsPageShoes:
			h += (m_cellheight * sm_shoes.count());
			break;
	}
	h += m_margin;
	return h;
}
