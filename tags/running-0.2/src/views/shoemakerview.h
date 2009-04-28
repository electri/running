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

#ifndef SHOEMAKERVIEW_H
#define SHOEMAKERVIEW_H

#include <QWidget>

#include "../../obj/ui_shoemakerview.h"

class ComboObjectTableModel;

class ShoeMakerView : public QDialog, public Ui::ShoeMakerView
{
	Q_OBJECT

public:
	ShoeMakerView(QWidget *parent = 0, quint32 id = 0);
	~ShoeMakerView();

private slots:
	void on_addPushButton_clicked();
	void on_removePushButton_clicked();
	void on_resetPushButton_clicked();
	void on_savePushButton_clicked();
	void on_cancelPushButton_clicked();

	void currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

	void on_descriptionLineEdit_textChanged(const QString &text);

private:
	void setControlsEnabled(bool enable);

	ComboObjectTableModel *m_model;
};

#endif
