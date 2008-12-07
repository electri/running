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

#ifndef SHOEMODELVIEW_H
#define SHOEMODELVIEW_H

#include <QWidget>

#include "../../obj/ui_shoemodelview.h"

class ShoeModelTableModel;

class ShoeModelView : public QDialog, public Ui::ShoeModelView
{
	Q_OBJECT

public:
	ShoeModelView(QWidget *parent = 0, quint32 id = 0);
	~ShoeModelView();

private slots:
	void on_addPushButton_clicked();
	void on_removePushButton_clicked();
	void on_resetPushButton_clicked();
	void on_savePushButton_clicked();
	void on_cancelPushButton_clicked();

	void currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

	void on_shoeMakerComboBox_currentIndexChanged(int index);
	void on_descriptionLineEdit_textChanged(const QString &text);

	void on_shoeMakerToolButton_clicked();

private:
	void setControlsEnabled(bool enable);

	ShoeModelTableModel *m_model;
};

#endif
