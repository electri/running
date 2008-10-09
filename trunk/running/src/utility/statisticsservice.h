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

#ifndef STATISTICSSERVICE_H
#define STATISTICSSERVICE_H

#include <QDateTime>
#include <QString>
#include <QSqlRecord>

namespace StatisticsResults {

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

}

namespace Utility {

class StatisticsService
{
public:
	virtual ~StatisticsService();

	static StatisticsService* instance();

	QList<StatisticsResults::EventsPerDate> eventsPerDate();
	QList<StatisticsResults::Shoes> shoes();

	StatisticsResults::EventsPerDate event(int year = 0, int month = 0);
	StatisticsResults::Shoes shoe(int id);

private:
	StatisticsService();

	StatisticsResults::EventsPerDate newEventResult(const QSqlRecord &);
	StatisticsResults::Shoes newShoeResult(const QSqlRecord &);

	static StatisticsService* sm_instance;
};

}

#endif
