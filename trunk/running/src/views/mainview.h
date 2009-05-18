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

#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>
#include "ui_mainview.h"
#include "objects/eventgateway.h"

class EventWidget;

class MainView : public QMainWindow, Ui::MainView
{
	Q_OBJECT

public:
	MainView(QWidget *parent = 0);
	~MainView();

private slots:
	void showCalendarAction();
	void on_calendarWidget_selectionChanged();
	void on_calendarWidget_activated();

	void showStatisticsAction();
	void on_statisticsPageEventsPushButton_clicked();
	void on_statisticsPageShoesPushButton_clicked();

	void addEventAction();
	void removeEventAction();
	void editEventAction();
	void on_eventWidget_accepted();
	void on_eventWidget_rejected();

	void settings();
	void systemInformations();
	void about();

private:
	void _addEvent(EventGateway &event);
	void _editEvent(EventGateway &event);
	void _showEventWidget(EventGateway &event);
	void _updateStatusbar();

	EventWidget *m_eventWidget;
};

#endif
