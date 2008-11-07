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

#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>

#include "../../obj/ui_mainview.h"

namespace Objects { class Event; }
namespace Services { class Memento; }

class VotePopupView;
class IntervalView;
class RunnerInfoPopupView;
class WeatherInfoPopupView;

class MainView : public QMainWindow, public Ui::MainView
{
	Q_OBJECT

public:
	MainView(QWidget *parent = 0);

private slots:
	void showCalendar();
	void on_calendarWidget_selectionChanged();
	void on_calendarWidget_currentPageChanged(int year, int month);

	void showStatistics();
	void on_statisticsPageEventsPushButton_clicked();
	void on_statisticsPageShoesPushButton_clicked();

	void addEvent();
	void removeEvent();
	void editEvent();
	void on_resetPushButton_clicked();
	void on_savePushButton_clicked();
	void on_cancelPushButton_clicked();
	void on_eventTypeComboBox_currentIndexChanged(int index);
	void on_distanceDoubleSpinBox_valueChanged(double value);
	void on_durationTimeEdit_timeChanged(const QTime &value);
	void on_eventTypeToolButton_clicked();
	void on_votePushButton_clicked();
	void on_intervalsPushButton_clicked();
	void on_runnerInfoPushButton_clicked();
	void on_weatherInfoPushButton_clicked();

	void editShoe(quint32 id = 0);
	void on_shoeToolButton_clicked();

	void editEventType(quint32 id = 0);
	void editShoeMaker(quint32 id = 0);
	void editShoeModel(quint32 id = 0);
	void editWeather(quint32 id = 0);
	void editIntervalType(quint32 id = 0);

	void options();
	void systemInformation();
	void about();
	void showDebugOnConsole();

private:
	void editEventBegin(Objects::Event *);
	void editEventEnd();
	void editEventGetProperties(Objects::Event *);
	void editEventSetProperties(Objects::Event *);
	void editEventRefreshComboBoxes();
	void refreshPaceLineEdit(double distance, const QTime &time);

	void updateStatusbar();

	Objects::Event *m_currentEvent;
	Services::Memento *m_memento;
	VotePopupView *m_votepopupview;
	IntervalView *m_intervalview;
	RunnerInfoPopupView *m_runnerinfopopupview;
	WeatherInfoPopupView *m_weatherinfopopupview;
};

#endif
