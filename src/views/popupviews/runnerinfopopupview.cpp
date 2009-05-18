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

#include <QtGui>
#include "runnerinfopopupview.h"
#include "settings.h"

RunnerInfoPopupView::RunnerInfoPopupView(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
}

void RunnerInfoPopupView::showEvent(QShowEvent *)
{
	weightDoubleSpinBox->setSuffix(QString(" %1").arg(Settings::instance()->weightUnit()));
}
