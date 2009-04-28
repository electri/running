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

#ifndef UTILITY_H
#define UTILITY_H

#include <QString>
#include <QDateTime>

class Utility
{
public:
	static QString formatDistance(qreal d, quint8 precision);
	static QString formatDuration(const QTime &t);
	static QString formatDuration(const QDateTime &t);

	static QTime paceTime(qreal distance, const QTime &duration);
	static QTime paceTime(qreal distance, const QDateTime &duration);

	static qreal paceSpeed(qreal distance, const QTime &duration);
	static qreal paceSpeed(qreal distance, const QDateTime &duration);
};

#endif
