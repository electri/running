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

#ifndef OPTIONSVIEW_H
#define OPTIONSVIEW_H

#include <QWidget>

#include "../../obj/ui_optionsview.h"

class QStringListModel;
namespace Objects { class Cfg; }
namespace Services { class Memento; }

class OptionsView : public QDialog, public Ui::OptionsView
{
	Q_OBJECT

public:
	OptionsView(QWidget *parent = 0);
	~OptionsView();

private slots:
	void on_resetPushButton_clicked();
	void on_savePushButton_clicked();
	void on_cancelPushButton_clicked();

	void on_treeView_clicked(const QModelIndex &);

private:
	void getProperties(Objects::Cfg *);
	void setProperties(Objects::Cfg *);
	void refreshComboBoxes();

	QStringListModel *m_model;
	Objects::Cfg *m_cfg;
	Services::Memento *m_memento;
};

#endif
