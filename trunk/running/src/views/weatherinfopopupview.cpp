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

#include "../objects/comboobject.h"
#include "../services/objectmap.h"
#include "../views/weatherview.h"

WeatherInfoPopupView::WeatherInfoPopupView(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
}



void WeatherInfoPopupView::on_weatherToolButton_clicked()
{
	quint32 weatherId = weatherComboBox->itemData(weatherComboBox->currentIndex()).toInt();

	WeatherView *view = new WeatherView(this, weatherId);
	int result = view->exec();
	if (result == QDialog::Accepted) {
		this->refreshComboBoxes();
	}
	delete view;
}



void WeatherInfoPopupView::refreshComboBoxes()
{
	Services::ObjectMap *session = Services::ObjectMap::instance();

	quint32 id = weatherComboBox->itemData(weatherComboBox->currentIndex()).toInt();
	weatherComboBox->clear();
	weatherComboBox->addItem("", 0);
	QList<Objects::BaseObject *> list = session->getAllObjects(Objects::Types::Weather);
	QList<Objects::BaseObject *>::const_iterator it = list.constBegin();
	while (it != list.constEnd()) {
		Objects::ComboObject *item = static_cast<Objects::ComboObject *>(*it++);
		weatherComboBox->addItem(item->description(), item->id());
	}
	session->discardObjects(list);
	weatherComboBox->setCurrentIndex(weatherComboBox->findData(id));
}
