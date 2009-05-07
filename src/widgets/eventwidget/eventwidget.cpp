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
#include "eventwidget.h"
#include "settings.h"
#include "utility/comboboxhelper.h"
#include "utility/completerhelper.h"
#include "utility/utility.h"
//#include "views/intervalview.h"
#include "views/popupviews/runnerinfopopupview.h"
#include "views/popupviews/votepopupview.h"
#include "views/popupviews/weatherinfopopupview.h"
#include "views/tableviews/eventtypeview.h"
#include "views/tableviews/shoeview.h"

EventWidget::EventWidget(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);

	starsSlider->setMinimum(0);
	starsSlider->setMaximum(5);
	starsSlider->setSingleStep(1);
	starsSlider->setPageStep(1);
	QPalette palette = starsSlider->palette();
	palette.setColor(QPalette::Highlight, QColor("yellow"));
	starsSlider->setPalette(palette);

//	m_intervalview = new IntervalView(event, this);
//	m_intervalview->setWindowFlags(Qt::Popup);
	m_runnerinfopopupview = new RunnerInfoPopupView(this);
	m_runnerinfopopupview->setWindowFlags(Qt::Popup);
	m_votepopupview = new VotePopupView(this);
	m_votepopupview->setWindowFlags(Qt::Popup);
	m_weatherinfopopupview = new WeatherInfoPopupView(this);
	m_weatherinfopopupview->setWindowFlags(Qt::Popup);

	m_event = EventGateway();
}

EventWidget::~EventWidget()
{
//	delete m_intervalview;
	delete m_runnerinfopopupview;
	delete m_votepopupview;
	delete m_weatherinfopopupview;
}

void EventWidget::applySettings()
{
	distanceDoubleSpinBox->setSuffix(" " + Settings::instance()->distanceUnit());
	paceLineEdit->setText(Utility::formatPace(m_event.distance(), m_event.duration()));
}

void EventWidget::reloadLists()
{
	nameLineEdit->setCompleter(CompleterHelper::completer("Event", "Name", nameLineEdit));
	descriptionLineEdit->setCompleter(CompleterHelper::completer("Event", "Description", descriptionLineEdit));

	ComboBoxHelper::fillComboBox(eventTypeComboBox, "EventType", false);
	ComboBoxHelper::fillShoesComboBox(shoeComboBox, false);
	m_weatherinfopopupview->reloadLists();
}

void EventWidget::setEvent(const EventGateway &event)
{
	reloadLists();

	m_event = event;
	setFields();

//	paceLineEdit->setText(Utility::formatPace(m_event.distance(), m_event.duration()));

	m_pendingDataChanges = false;
}

EventGateway EventWidget::event() const
{
	return m_event;
}

void EventWidget::setFields()
{
	ComboBoxHelper::setSelected(eventTypeComboBox, m_event.eventType_id());
	starsSlider->setValue(m_event.vote());
	m_votepopupview->qualitySpinBox->setValue(m_event.quality());
	m_votepopupview->effortSpinBox->setValue(m_event.effort());
	startTimeEdit->setTime(m_event.start().time());
	nameLineEdit->setText(m_event.name());
	descriptionLineEdit->setText(m_event.description());
	distanceDoubleSpinBox->setValue(m_event.distance());
	durationTimeEdit->setTime(m_event.duration());
	ComboBoxHelper::setSelected(shoeComboBox, m_event.shoe_id());
	notesPlainTextEdit->setPlainText(m_event.notes());
	m_runnerinfopopupview->weightDoubleSpinBox->setValue(m_event.weight());
	ComboBoxHelper::setSelected(m_weatherinfopopupview->weatherComboBox, m_event.weather_id());
	m_weatherinfopopupview->temperatureDoubleSpinBox->setValue(m_event.temperature());
}

void EventWidget::getFields()
{
	m_event.setEventType_id(ComboBoxHelper::selected(eventTypeComboBox));
	m_event.setVote(starsSlider->value());
	m_event.setQuality(m_votepopupview->qualitySpinBox->value());
	m_event.setEffort(m_votepopupview->effortSpinBox->value());
	m_event.setStart(QDateTime(m_event.start().date(), startTimeEdit->time()));
	m_event.setName(nameLineEdit->text());
	m_event.setDescription(descriptionLineEdit->text());
	m_event.setDistance(distanceDoubleSpinBox->value());
	m_event.setDuration(durationTimeEdit->time());
	m_event.setShoe_id(ComboBoxHelper::selected(shoeComboBox));
	m_event.setNotes(notesPlainTextEdit->toPlainText());
	m_event.setWeight(m_runnerinfopopupview->weightDoubleSpinBox->value());
	m_event.setWeather_id(ComboBoxHelper::selected(m_weatherinfopopupview->weatherComboBox));
	m_event.setTemperature(m_weatherinfopopupview->temperatureDoubleSpinBox->value());
}

void EventWidget::on_resetPushButton_clicked()
{
	if (m_pendingDataChanges) {
		if (QMessageBox::question(this, tr("Reset"), tr("There are pending changes to save. Are you sure to reset?"), QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes) return;
	}

//	m_intervalview->resetAll();

	setFields();
	m_pendingDataChanges = false;
}

void EventWidget::on_savePushButton_clicked()
{
	getFields();

//	m_intervalview->saveAll();

	if (!m_event.save()) {
		QMessageBox::critical(this, tr("Add/Edit an event"), m_event.lastError());
		return;
	}

	emit accept();
}

void EventWidget::on_cancelPushButton_clicked()
{
	if (m_pendingDataChanges) {
		if (QMessageBox::question(this, tr("Cancel"), tr("There are pending changes to save. Are you sure to cancel?"), QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes) return;
	}

	emit reject();
}

void EventWidget::on_distanceDoubleSpinBox_valueChanged(double value)
{
	QTime time = durationTimeEdit->time();
	paceLineEdit->setText(Utility::formatPace(value, time));
}

void EventWidget::on_durationTimeEdit_timeChanged(const QTime &value)
{
	double distance = distanceDoubleSpinBox->value();
	paceLineEdit->setText(Utility::formatPace(distance, value));
}

void EventWidget::on_eventTypeComboBox_currentIndexChanged(int)
{

}

void EventWidget::on_eventTypeToolButton_clicked()
{
	quint32 id = ComboBoxHelper::selected(eventTypeComboBox);

	EventTypeView *view = new EventTypeView(this, id);
	int result = view->exec();
	if (result == QDialog::Accepted) {
//		statisticsWidget->refreshCache();
//		calendarWidget->update();
		ComboBoxHelper::fillComboBox(eventTypeComboBox, "EventType", false);
	}
	delete view;
}

void EventWidget::on_shoeToolButton_clicked()
{
	quint32 id = ComboBoxHelper::selected(shoeComboBox);

	ShoeView *view = new ShoeView(this, id);
	int result = view->exec();
	if (result == QDialog::Accepted) {
//		statisticsWidget->refreshCache();
//		calendarWidget->update();
		ComboBoxHelper::fillShoesComboBox(shoeComboBox, false);
	}
	delete view;
}

void EventWidget::on_intervalsPushButton_clicked()
{
//	m_intervalview->move(ui->intervalsPushButton->mapToGlobal(QPoint(0, (m_intervalview->height() + 10) * -1)));
//	m_intervalview->show();
}

void EventWidget::on_runnerInfoPushButton_clicked()
{
	m_runnerinfopopupview->move(runnerInfoPushButton->mapToGlobal(QPoint(0, (m_runnerinfopopupview->height() + 10) * -1)));
	m_runnerinfopopupview->show();
}

void EventWidget::on_votePushButton_clicked()
{
	m_votepopupview->move(votePushButton->mapToGlobal(QPoint(0, votePushButton->height() + 10)));
	m_votepopupview->show();
}

void EventWidget::on_weatherInfoPushButton_clicked()
{
	m_weatherinfopopupview->move(weatherInfoPushButton->mapToGlobal(QPoint(0, (m_weatherinfopopupview->height() + 10) * -1)));
	m_weatherinfopopupview->show();
}
