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

#include <QtGui>

#include "weatherinfopopupview.h"

#include "../application.h"
#include "../objects/comboobject.h"
#include "../views/weatherview.h"
#include "../views/viewhelper.h"

WeatherInfoPopupView::WeatherInfoPopupView(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
}

void WeatherInfoPopupView::showEvent(QShowEvent *)
{
	temperatureDoubleSpinBox->setSuffix(" " + Application::instance()->cfg()->cfgTemperatureUnit()->description());
}



void WeatherInfoPopupView::on_weatherToolButton_clicked()
{
	quint32 id = weatherComboBox->itemData(weatherComboBox->currentIndex()).toInt();

	WeatherView *view = new WeatherView(this, id);
	int result = view->exec();
	if (result == QDialog::Accepted) {
		this->refreshComboBoxes();
	}
	delete view;
}



void WeatherInfoPopupView::refreshComboBoxes()
{
	ViewHelper::fillComboBox(weatherComboBox, Objects::Types::Weather);
	weatherComboBox->insertItem(0, "", 0);
}
