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

#ifndef WEATHERVIEW_H
#define WEATHERVIEW_H

#include "abstracttableview.h"
#include "ui_weatherview.h"

class WeatherView : public AbstractTableView, public Ui::WeatherView
{
	Q_OBJECT

public:
	WeatherView(QWidget *parent = 0, quint32 id = 0);
	~WeatherView();

private slots:
	void currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

	void on_descriptionLineEdit_textChanged(const QString &value);

private:
	void setControlsEnabled(bool enable);
};

#endif // WEATHERVIEW_H
