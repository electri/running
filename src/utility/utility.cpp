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

#include "utility.h"

QString Utility::formatDistance(qreal d, quint8 precision)
{
	QString s = "0";
	s = QString("%L1").arg(d, 0, 'f', precision);
	while ((s.contains(",") || s.contains(".")) && s.endsWith("0")) {
		if (s.endsWith("0")) s.chop(1);
		if (s.endsWith(",") || s.endsWith(".")) s.chop(1);
	}
	return s;
}

QString Utility::formatDuration(const QTime &t)
{
	QString s = "00\"";
	if (t.isValid()) {
		if (t.hour() > 0) {
			s = t.toString("h'h' mm'' ss'\"'");
		} else {
			if (t.minute() > 0) {
				s = t.toString("m'' ss'\"'");
			} else {
				s = t.toString("s'\"'");
			}
		}
	}
	return s;
}

QString Utility::formatDuration(const QDateTime &t)
{
	QString s = "00\"";
	if (t.isValid()) {
		qint32 days = QDateTime(QDate(1970, 1, 1)).daysTo(t);
		if (days > 0) {
			s = QString("%1g ").arg(days) + t.time().toString("h'h' mm'' ss'\"'");
		} else {
			s = formatDuration(t.time());
		}
	}
	return s;
}

QTime Utility::paceTime(qreal distance, const QTime &duration)
{
	QTime t = QTime();
	if ((distance > 0.0) && (duration > QTime(0, 0, 0))) {
		qint32 seconds = (duration.hour() * 3600) + (duration.minute() * 60) + duration.second();

		qint32 i = (qint32)((qreal)seconds / distance);
		t = QTime((i / 3600), ((i % 3600) / 60), (i % 60));
	}
	return t;
}

QTime Utility::paceTime(qreal distance, const QDateTime &duration)
{
	QTime t = QTime();
	if ((distance > 0.0) && (duration > QDateTime(QDate(1970, 1, 1), QTime(0, 0, 0)))) {
		qint32 days = QDateTime(QDate(1970, 1, 1)).daysTo(duration);
		if (days > 0) {
			QTime t1 = duration.time();

			qint32 seconds = (days * 86400) + (t1.hour() * 3600) + (t1.minute() * 60) + t1.second();

			qint32 i = (qint32)((qreal)seconds / distance);
			t = QTime((i / 3600), ((i % 3600) / 60), (i % 60));
		} else {
			return paceTime(distance, duration.time());
		}
	}
	return t;
}

qreal Utility::paceSpeed(qreal distance, const QTime &duration)
{
	qreal d = 0.0;
	if ((distance > 0.0) && (duration > QTime(0, 0, 0))) {
		qint32 seconds = (duration.hour() * 3600) + (duration.minute() * 60) + duration.second();

		d = (distance / seconds) * 3600;
	}
	return d;
}

qreal Utility::paceSpeed(qreal distance, const QDateTime &duration)
{
	qreal d = 0.0;
	if ((distance > 0.0) && duration.isValid()) {
		qint32 days = QDateTime(QDate(1970, 1, 1)).daysTo(duration);
		if (days > 0) {
			QTime t1 = duration.time();

			qint32 seconds = (days * 86400) + (t1.hour() * 3600) + (t1.minute() * 60) + t1.second();

			d = (distance / seconds) * 3600;

		} else {
			return paceSpeed(distance, duration.time());
		}
	}
	return d;
}
