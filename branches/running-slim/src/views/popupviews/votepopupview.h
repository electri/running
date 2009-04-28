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

#ifndef VOTEPOPUPVIEW_H
#define VOTEPOPUPVIEW_H

#include <QWidget>
#include "ui_votepopupview.h"

class VotePopupView : public QWidget, public Ui::VotePopupView
{
	Q_OBJECT

public:
	VotePopupView(QWidget *parent = 0);
};

#endif // VOTEPOPUPVIEW_H
