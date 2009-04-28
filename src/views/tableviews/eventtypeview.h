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

#ifndef EVENTTYPEVIEW_H
#define EVENTTYPEVIEW_H

#include "abstracttableview.h"
#include "ui_eventtypeview.h"

class EventTypeView : public AbstractTableView, public Ui::EventTypeView
{
	Q_OBJECT

public:
	EventTypeView(QWidget *parent = 0, quint32 id = 0);
	~EventTypeView();

private slots:
	void currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

	void on_descriptionLineEdit_textChanged(const QString &value);
	void on_hasMedalCheckBox_stateChanged(int value);
	void on_hasIntervalsCheckBox_stateChanged(int value);

private:
	void setControlsEnabled(bool value);
};

#endif // EVENTTYPEVIEW_H
