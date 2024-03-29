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

#ifndef SHOEVIEW_H
#define SHOEVIEW_H

#include "abstracttableview.h"
#include "ui_shoeview.h"

class ShoeView : public AbstractTableView, public Ui::ShoeView
{
	Q_OBJECT

public:
	ShoeView(QWidget *parent = 0, quint32 id = 0);
	~ShoeView();

	void showEvent(QShowEvent *);

private slots:
	void currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

	void on_shoeMakerComboBox_currentIndexChanged(int index);
	void on_shoeModelComboBox_currentIndexChanged(int index);
	void on_initialDistanceDoubleSpinBox_valueChanged(double value);
	void on_purchaseDateDateEdit_dateChanged(const QDate &value);
	void on_sizeDoubleSpinBox_valueChanged(double value);
	void on_priceDoubleSpinBox_valueChanged(double value);
	void on_retiredCheckBox_stateChanged(int state);
	void on_notesPlainTextEdit_textChanged();

	void on_shoeMakerToolButton_clicked();
	void on_shoeModelToolButton_clicked();

private:
	void setControlsEnabled(bool enable);

//	double m_distance;
};

#endif // SHOEVIEW_H
