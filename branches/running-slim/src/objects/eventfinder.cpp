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

#include <QtSql>
#include "eventfinder.h"

bool EventFinder::find(EventGateway &event, quint32 id)
{
	QString queryText = QString("SELECT Event.Id, Event.Start, Event.Name, Event.Description, EventType.Id AS EventType_Id, EventType.Description AS EventType_Description, EventType.HasMedal AS EventType_HasMedal, Event.Distance, Event.Duration, Event.Notes, Shoe.Id AS Shoe_Id, ShoeMaker.Description || ' ' || ShoeModel.Description AS Shoe_Description, Event.Vote, Event.Quality, Event.Effort, Event.Weight, Weather.Id AS Weather_Id, Weather.Description AS Weather_Description, Event.Temperature FROM Event LEFT JOIN EventType ON EventType.Id = Event.EventTypeId LEFT JOIN Shoe ON Shoe.Id = Event.ShoeId LEFT JOIN ShoeModel ON ShoeModel.Id = Shoe.ShoeModelId LEFT JOIN ShoeMaker ON ShoeMaker.Id = ShoeModel.ShoeMakerId LEFT JOIN Weather ON Weather.Id = Event.WeatherId WHERE Event.Id = %1").arg(id);

	QSqlDatabase db = QSqlDatabase::database();
	if (db.isValid()) {
		if (db.isOpen()) {
			QSqlQuery query;
			query.exec(queryText);
			if (query.first()) {
				event._load(query.record());
				return true;
			}
		}
	}

	return false;
}

bool EventFinder::find(EventGateway &event, const QDate &date)
{
	QString queryText = QString("SELECT Event.Id, Event.Start, Event.Name, Event.Description, EventType.Id AS EventType_Id, EventType.Description AS EventType_Description, EventType.HasMedal AS EventType_HasMedal, Event.Distance, Event.Duration, Event.Notes, Shoe.Id AS Shoe_Id, ShoeMaker.Description || ' ' || ShoeModel.Description AS Shoe_Description, Event.Vote, Event.Quality, Event.Effort, Event.Weight, Weather.Id AS Weather_Id, Weather.Description AS Weather_Description, Event.Temperature FROM Event LEFT JOIN EventType ON EventType.Id = Event.EventTypeId LEFT JOIN Shoe ON Shoe.Id = Event.ShoeId LEFT JOIN ShoeModel ON ShoeModel.Id = Shoe.ShoeModelId LEFT JOIN ShoeMaker ON ShoeMaker.Id = ShoeModel.ShoeMakerId LEFT JOIN Weather ON Weather.Id = Event.WeatherId WHERE CAST(strftime('%Y',Event.Start) AS INTEGER) = %1 AND CAST(strftime('%m',Event.Start) AS INTEGER) = %2 AND CAST(strftime('%d',Event.Start) AS INTEGER) = %3").arg(date.year()).arg(date.month()).arg(date.day());

	QSqlDatabase db = QSqlDatabase::database();
	if (db.isValid()) {
		if (db.isOpen()) {
			QSqlQuery query;
			query.exec(queryText);
			if (query.first()) {
				event._load(query.record());
				return true;
			}
		}
	}

	return false;
}
