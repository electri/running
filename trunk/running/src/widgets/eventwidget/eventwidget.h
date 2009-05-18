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

#ifndef EVENTWIDGET_H
#define EVENTWIDGET_H

#include <QWidget>
#include <QDate>
#include "ui_eventwidget.h"
#include "objects/eventgateway.h"

class IntervalPopupView;
class RunnerInfoPopupView;
class VotePopupView;
class WeatherInfoPopupView;

class EventWidget : public QWidget, Ui::EventWidget
{
	Q_OBJECT

public:
	EventWidget(const EventGateway &event, QWidget *parent = 0);
	~EventWidget();

	void showEvent(QShowEvent *);

signals:
	void accepted();
	void rejected();

public slots:
	void on_resetPushButton_clicked();
	void on_savePushButton_clicked();
	void on_cancelPushButton_clicked();

private slots:
	void on_distanceDoubleSpinBox_valueChanged(double value);
	void on_durationTimeEdit_timeChanged(const QTime &value);
	void on_eventTypeComboBox_currentIndexChanged(int index);
	void on_eventTypeToolButton_clicked();
	void on_intervalsPushButton_clicked();
	void on_runnerInfoPushButton_clicked();
	void on_votePushButton_clicked();
	void on_weatherInfoPushButton_clicked();
	void on_shoeToolButton_clicked();

private:
	void _setFields();
	void _getFields();

	EventGateway m_event;
	bool m_pendingDataChanges;
	IntervalPopupView *m_intervalpopupview;
	RunnerInfoPopupView *m_runnerinfopopupview;
	VotePopupView *m_votepopupview;
	WeatherInfoPopupView *m_weatherinfopopupview;
};

#endif // EVENTWIDGET_H
