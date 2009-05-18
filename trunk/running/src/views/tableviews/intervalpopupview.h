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

#ifndef INTERVALPOPUPVIEW_H
#define INTERVALPOPUPVIEW_H

#include <QSqlRecord>
#include "abstracttableview.h"
#include "ui_intervalpopupview.h"

class EventGateway;
class QSqlTableModel;

class IntervalPopupView : public AbstractTableView, public Ui::IntervalPopupView
{
	Q_OBJECT

public:
	IntervalPopupView(EventGateway *event, QWidget *parent = 0);
	~IntervalPopupView();

	void showEvent(QShowEvent *);

	void setEvent(EventGateway *event);
	EventGateway *event() const;
	QSqlTableModel *model() const;

	void resetAll();
	bool saveAll();

private slots:
	void currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

	void on_rowUpPushButton_clicked();
	void on_rowDownPushButton_clicked();

	void on_intervalTypeComboBox_currentIndexChanged(int index);
	void on_distanceDoubleSpinBox_valueChanged(double value);
	void on_durationTimeEdit_timeChanged(const QTime &value);
	void on_notesPlainTextEdit_textChanged();

	void on_intervalTypeToolButton_clicked();

private:
	void setControlsEnabled(bool enable);

	EventGateway *m_event;
};

#endif // INTERVALPOPUPVIEW_H
