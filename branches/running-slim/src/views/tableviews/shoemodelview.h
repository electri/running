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

#ifndef SHOEMODELVIEW_H
#define SHOEMODELVIEW_H

#include "abstracttableview.h"
#include "ui_shoemodelview.h"

class ShoeModelView : public AbstractTableView, public Ui::ShoeModelView
{
	Q_OBJECT

public:
	ShoeModelView(QWidget *parent = 0, quint32 id = 0);
	~ShoeModelView();

private slots:
	void currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

	void on_shoeMakerComboBox_currentIndexChanged(int index);
	void on_descriptionLineEdit_textChanged(const QString &value);

	void on_shoeMakerToolButton_clicked();

private:
	void setControlsEnabled(bool enable);
};

#endif
