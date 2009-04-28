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

#ifndef INTERVALVIEW_H
#define INTERVALVIEW_H

#include <QWidget>

#include "../../obj/ui_intervalview.h"

namespace Objects { class Event; }
class IntervalTableModel;

class IntervalView : public QWidget, public Ui::IntervalView
{
	Q_OBJECT

public:
	IntervalView(Objects::Event *event, QWidget *parent = 0, quint32 id = 0);
	~IntervalView();
	void showEvent(QShowEvent *);

	void resetAll();
	bool saveAll();

private slots:
	void on_addPushButton_clicked();
	void on_removePushButton_clicked();

	void on_rowUpPushButton_clicked();
	void on_rowDownPushButton_clicked();

	void currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

	void on_intervalTypeComboBox_currentIndexChanged(int index);
	void on_distanceDoubleSpinBox_valueChanged(double value);
	void on_durationTimeEdit_timeChanged(const QTime &value);
	void on_notesPlainTextEdit_textChanged();

	void on_intervalTypeToolButton_clicked();

private:
	void setControlsEnabled(bool enable);
	void swapRows(int row1, int row2);
	void refreshPaceLineEdit(double distance, QTime time);

	IntervalTableModel *m_model;
};

#endif
