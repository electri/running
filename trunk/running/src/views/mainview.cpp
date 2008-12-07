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

#include "mainview.h"

#include "../application.h"
#include "../objects/event.h"
#include "../objects/eventtype.h"
#include "../objects/shoe.h"
#include "../objects/weather.h"
#include "../services/memento.h"
#include "../utility/statisticsservice.h"
#include "../utility/utility.h"
#include "../views/votepopupview.h"
#include "../views/runnerinfopopupview.h"
#include "../views/weatherinfopopupview.h"
#include "../views/eventtypeview.h"
#include "../views/shoeview.h"
#include "../views/shoemakerview.h"
#include "../views/shoemodelview.h"
#include "../views/weatherview.h"
#include "../views/intervalview.h"
#include "../views/intervaltypeview.h"
#include "../views/optionsview.h"
#include "../views/viewhelper.h"
#include "../widgets/qxtstars/qxtstars.h"

MainView::MainView(QWidget *parent)
	: QMainWindow(parent)
{
	setupUi(this);

	m_currentEvent = NULL;
	m_memento = NULL;
	m_votepopupview = NULL;
	m_intervalview = NULL;
	m_runnerinfopopupview = NULL;
	m_weatherinfopopupview = NULL;

	starsSlider->setMinimum(0);
	starsSlider->setMaximum(5);
	starsSlider->setSingleStep(1);
	starsSlider->setPageStep(1);
	QPalette palette = starsSlider->palette();
	palette.setColor(QPalette::Highlight, QColor("yellow"));
	starsSlider->setPalette(palette);

	connect(actionSystemInformation, SIGNAL(triggered()), this, SLOT(systemInformation()));
	connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));

	if (Application::instance()->init()) {
		calendarWidget->setFirstDayOfWeek(Application::instance()->cfg()->isMondayFirstDayOfWeek() ? Qt::Monday : Qt::Sunday);

		connect(actionShowCalendar, SIGNAL(triggered()), this, SLOT(showCalendar()));
		connect(actionShowStatistics, SIGNAL(triggered()), this, SLOT(showStatistics()));
		connect(actionAdd, SIGNAL(triggered()), this, SLOT(addEvent()));
		connect(actionRemove, SIGNAL(triggered()), this, SLOT(removeEvent()));
		connect(actionEdit, SIGNAL(triggered()), this, SLOT(editEvent()));
		connect(actionEditShoe, SIGNAL(triggered()), this, SLOT(editShoe()));
		connect(actionEditEventType, SIGNAL(triggered()), this, SLOT(editEventType()));
		connect(actionEditShoeMaker, SIGNAL(triggered()), this, SLOT(editShoeMaker()));
		connect(actionEditShoeModel, SIGNAL(triggered()), this, SLOT(editShoeModel()));
		connect(actionEditWeather, SIGNAL(triggered()), this, SLOT(editWeather()));
		connect(actionEditIntervalType, SIGNAL(triggered()), this, SLOT(editIntervalType()));
		connect(actionOptions, SIGNAL(triggered()), this, SLOT(options()));
	} else {
		actionShowCalendar->setEnabled(false);
		actionShowStatistics->setEnabled(false);
		actionAdd->setEnabled(false);
		actionRemove->setEnabled(false);
		actionEdit->setEnabled(false);
		actionEditShoe->setEnabled(false);
		actionEditEventType->setEnabled(false);
		actionEditShoeMaker->setEnabled(false);
		actionEditShoeModel->setEnabled(false);
		actionEditWeather->setEnabled(false);
		actionEditIntervalType->setEnabled(false);

		QMessageBox::critical(0, tr("Error"), tr("Unable to open or read the database file."), QMessageBox::Ok, QMessageBox::NoButton);
	}

#ifdef QT_DEBUG
//	calendarWidget->setSelectedDate(QDate(2006, 11, 1));
#endif
	on_calendarWidget_selectionChanged();

#ifdef QT_DEBUG
	QTimer *timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(showDebugOnConsole()));
	timer->start(3000);
#endif
}



// CALENDAR WIDGET

void MainView::showCalendar()
{
	stackedWidget->setCurrentIndex(stackedWidget->indexOf(calendarPage));

	actionShowCalendar->setEnabled(true);
	actionShowStatistics->setEnabled(true);
	actionAdd->setEnabled(true);
	actionEdit->setEnabled(true);
	actionRemove->setEnabled(true);

	this->on_calendarWidget_selectionChanged();
}

void MainView::on_calendarWidget_selectionChanged()
{
	Services::ObjectMap *session = Application::instance()->objectMap();

	if (m_currentEvent) {
		session->discardObject(m_currentEvent);
		m_currentEvent = NULL;
	}

	QDate date = calendarWidget->selectedDate();
	QList<Objects::BaseObject *> list = session->getEventsByDate(date, date);
	if (!list.isEmpty()) {
		m_currentEvent = static_cast<Objects::Event *>(list.takeFirst());
	}
	if (!list.isEmpty()) {
		foreach (Objects::BaseObject *object, list) {
			session->discardObject(object);
		}
	}

	this->updateStatusbar();
}

void MainView::on_calendarWidget_currentPageChanged(int year, int month)
{
	calendarWidget->setSelectedDate(QDate(year, month, 1));
	this->on_calendarWidget_selectionChanged();
}

// STATISTICS WIDGET

void MainView::showStatistics()
{
	stackedWidget->setCurrentIndex(stackedWidget->indexOf(statisticsPage));
	statisticsWidget->setPage(StatisticsPages::EventsPerDate);

	actionAdd->setEnabled(false);
	actionEdit->setEnabled(false);
	actionRemove->setEnabled(false);

	QString message = tr("Calendar statistics.");
	statusbar->showMessage(message);
}

void MainView::on_statisticsPageEventsPushButton_clicked()
{
	statisticsWidget->setPage(StatisticsPages::EventsPerDate);
}

void MainView::on_statisticsPageShoesPushButton_clicked()
{
	statisticsWidget->setPage(StatisticsPages::Shoes);
}

// EDIT EVENT

void MainView::addEvent()
{
	if (m_currentEvent) {
		QMessageBox::warning(this, tr("Add a new event"), tr("The selected day already has an event."));
	} else {
		m_currentEvent = static_cast<Objects::Event *>(Application::instance()->objectMap()->createObject(Objects::Types::Event));
		m_currentEvent->setStart(QDateTime(calendarWidget->selectedDate(), QTime()));

		editEventBegin(m_currentEvent);

		QString message = tr("Add event: %1.").arg(m_currentEvent->start().date().toString("d MMMM yyyy"));
		statusbar->showMessage(message);
	}
}

void MainView::removeEvent()
{
	if (m_currentEvent)	{
		if (QMessageBox::question(this, tr("Remove an event"), tr("Are you sure you want to remove the event: %1?")
				.arg(m_currentEvent->start().date().toString("d MMMM yyyy")),
				QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No) {
			return;
		}

		if (!Application::instance()->objectMap()->eraseObject(m_currentEvent)) {
			QMessageBox::critical(this, tr("Remove an event"), Application::instance()->objectMap()->lastError());
		}

		this->on_calendarWidget_selectionChanged();
	} else {
		QMessageBox::warning(this, tr("Remove an event"), tr("The selected day don't have an event."));
	}
}

void MainView::editEvent()
{
	if (m_currentEvent) {
		editEventBegin(m_currentEvent);

		QString message = tr("Edit event: %1.").arg(m_currentEvent->start().date().toString("d MMMM yyyy"));
		statusbar->showMessage(message);
	} else {
		QMessageBox::warning(this, tr("Edit an event"), tr("The selected day don't have an event."));
	}
}

void MainView::editEventBegin(Objects::Event *object)
{
	m_memento = new Services::Memento(object);

	Objects::Event *event = static_cast<Objects::Event *>(m_memento->copy());

	m_votepopupview = new VotePopupView(this);
	m_votepopupview->setWindowFlags(Qt::Popup);

	m_intervalview = new IntervalView(event, this);
	m_intervalview->setWindowFlags(Qt::Popup);

	m_runnerinfopopupview = new RunnerInfoPopupView(this);
	m_runnerinfopopupview->setWindowFlags(Qt::Popup);

	m_weatherinfopopupview = new WeatherInfoPopupView(this);
	m_weatherinfopopupview->setWindowFlags(Qt::Popup);

	editEventRefreshComboBoxes();

	editEventSetProperties(event);

	stackedWidget->setCurrentIndex(stackedWidget->indexOf(eventPage));

	actionShowCalendar->setEnabled(false);
	actionShowStatistics->setEnabled(false);
	actionAdd->setEnabled(false);
	actionEdit->setEnabled(false);
	actionRemove->setEnabled(false);

	nameLineEdit->setCompleter(ViewHelper::completer(nameLineEdit, "Event", "Name"));
	descriptionLineEdit->setCompleter(ViewHelper::completer(descriptionLineEdit, "Event", "Description"));

	distanceDoubleSpinBox->setSuffix(" " + Application::instance()->cfg()->cfgDistanceUnit()->description());
}

void MainView::editEventEnd()
{
	delete m_votepopupview;
	delete m_intervalview;
	delete m_runnerinfopopupview;
	delete m_weatherinfopopupview;

	delete m_memento;

	this->showCalendar();
}

void MainView::editEventGetProperties(Objects::Event *object)
{
	if (object) {
		object->setStart(QDateTime(object->start().date(), startTimeEdit->time()));
		object->setName(nameLineEdit->text());
		object->setDescription(descriptionLineEdit->text());
		object->setEventType(static_cast<Objects::EventType *>(ViewHelper::getObjectOnComboBox(eventTypeComboBox, Objects::Types::EventType, object->eventType())));
		object->setDistance(distanceDoubleSpinBox->value());
		object->setDuration(durationTimeEdit->time());
		object->setNotes(notesPlainTextEdit->toPlainText());
		object->setShoe(static_cast<Objects::Shoe *>(ViewHelper::getObjectOnComboBox(shoeComboBox, Objects::Types::Shoe, object->shoe())));
		object->setVote(starsSlider->value());
		object->setQuality(m_votepopupview->qualitySpinBox->value());
		object->setEffort(m_votepopupview->effortSpinBox->value());
		object->setWeight(m_runnerinfopopupview->weightDoubleSpinBox->value());
		object->setWeather(static_cast<Objects::Weather *>(ViewHelper::getObjectOnComboBox(m_weatherinfopopupview->weatherComboBox, Objects::Types::Weather, object->weather())));
		object->setTemperature(m_weatherinfopopupview->temperatureDoubleSpinBox->value());
	}
}

void MainView::editEventSetProperties(Objects::Event *object)
{
	if (object) {
		startTimeEdit->setTime(object->start().time());
		nameLineEdit->setText(object->name());
		descriptionLineEdit->setText(object->description());
		ViewHelper::setIndexOnComboBox(eventTypeComboBox, object->eventType());
		distanceDoubleSpinBox->setValue(object->distance());
		durationTimeEdit->setTime(object->duration());
		notesPlainTextEdit->setPlainText(object->notes());
		ViewHelper::setIndexOnComboBox(shoeComboBox, object->shoe());
		starsSlider->setValue(object->vote());
		m_votepopupview->qualitySpinBox->setValue(object->quality());
		m_votepopupview->effortSpinBox->setValue(object->effort());
		m_runnerinfopopupview->weightDoubleSpinBox->setValue(object->weight());
		ViewHelper::setIndexOnComboBox(m_weatherinfopopupview->weatherComboBox, object->weather());
		m_weatherinfopopupview->temperatureDoubleSpinBox->setValue(object->temperature());
	}
}

void MainView::editEventRefreshComboBoxes()
{
	ViewHelper::fillComboBox(eventTypeComboBox, Objects::Types::EventType, false);
	ViewHelper::fillShoesComboBox(shoeComboBox);
}

void MainView::on_resetPushButton_clicked()
{
	m_intervalview->resetAll();

	m_memento->revert();

	Objects::Event *event = static_cast<Objects::Event *>(m_memento->copy());
	editEventSetProperties(event);
}

void MainView::on_savePushButton_clicked()
{
	Objects::Event *event = static_cast<Objects::Event *>(m_memento->copy());
	editEventGetProperties(event);

	m_intervalview->saveAll();

	if (!Application::instance()->objectMap()->saveObject(m_memento->copy())) {
		QMessageBox::critical(this, tr("Add/Edit an event"), Application::instance()->objectMap()->lastError());
		return;
	}
	m_memento->submit();

	editEventEnd();
}

void MainView::on_cancelPushButton_clicked()
{
	editEventEnd();
}

void MainView::on_eventTypeComboBox_currentIndexChanged(int)
{
	Services::ObjectMap *session = Application::instance()->objectMap();

	quint32 eventTypeId = eventTypeComboBox->itemData(eventTypeComboBox->currentIndex()).toInt();
	if (eventTypeId != 0) {
		Objects::EventType *eventType = static_cast<Objects::EventType *>(session->getObjectById(Objects::Types::EventType, eventTypeId));
		if (eventType) {
			intervalsPushButton->setEnabled(eventType->hasIntervals());
			session->discardObject(eventType);
		}
	} else {
		intervalsPushButton->setEnabled(false);
	}
}

void MainView::on_distanceDoubleSpinBox_valueChanged(double value)
{
	QTime time = durationTimeEdit->time();
	this->refreshPaceLineEdit(value, time);
}

void MainView::on_durationTimeEdit_timeChanged(const QTime &value)
{
	double distance = distanceDoubleSpinBox->value();
	this->refreshPaceLineEdit(distance, value);
}

void MainView::refreshPaceLineEdit(double distance, const QTime &time)
{
	QTime paceTime = Utility::paceTime(distance, time);
	double paceSpeed = Utility::paceSpeed(distance, time);
	paceLineEdit->setText(tr("%1 min/%3 or %2 %3/h")
			.arg(Utility::formatDuration(paceTime)).arg(Utility::formatDistance(paceSpeed, 2)).arg(Application::instance()->cfg()->cfgDistanceUnit()->description()));
}

void MainView::on_eventTypeToolButton_clicked()
{
	quint32 id = eventTypeComboBox->itemData(eventTypeComboBox->currentIndex()).toInt();
	this->editEventType(id);
}

void MainView::on_votePushButton_clicked()
{
	m_votepopupview->move(votePushButton->mapToGlobal(QPoint(0, votePushButton->height() + 10)));
	m_votepopupview->show();
}

void MainView::on_intervalsPushButton_clicked()
{
	m_intervalview->move(intervalsPushButton->mapToGlobal(QPoint(0, (m_intervalview->height() + 10) * -1)));
	m_intervalview->show();
}

void MainView::on_runnerInfoPushButton_clicked()
{
	m_runnerinfopopupview->move(runnerInfoPushButton->mapToGlobal(QPoint(0, (m_runnerinfopopupview->height() + 10) * -1)));
	m_runnerinfopopupview->show();
}

void MainView::on_weatherInfoPushButton_clicked()
{
	m_weatherinfopopupview->move(weatherInfoPushButton->mapToGlobal(QPoint(0, (m_weatherinfopopupview->height() + 10) * -1)));
	m_weatherinfopopupview->show();
}

// EDIT SHOE

void MainView::editShoe(quint32 id)
{
	ShoeView *view = new ShoeView(this, id);
	int result = view->exec();
	if (result == QDialog::Accepted) {
		calendarWidget->update();
	}
	delete view;

	this->editEventRefreshComboBoxes();
}

void MainView::on_shoeToolButton_clicked()
{
	quint32 id = shoeComboBox->itemData(shoeComboBox->currentIndex()).toInt();
	this->editShoe(id);
}

// EDIT COMBO OBJECTS

void MainView::editEventType(quint32 id)
{
	EventTypeView *view = new EventTypeView(this, id);
	int result = view->exec();
	if (result == QDialog::Accepted) {
		calendarWidget->update();
	}
	delete view;

	this->editEventRefreshComboBoxes();
}

void MainView::editShoeMaker(quint32 id)
{
	ShoeMakerView *view = new ShoeMakerView(this, id);
	int result = view->exec();
	if (result == QDialog::Accepted) {
		calendarWidget->update();
	}
	delete view;

	this->editEventRefreshComboBoxes();
}

void MainView::editShoeModel(quint32 id)
{
	ShoeModelView *view = new ShoeModelView(this, id);
	int result = view->exec();
	if (result == QDialog::Accepted) {
		calendarWidget->update();
	}
	delete view;

	this->editEventRefreshComboBoxes();
}

void MainView::editWeather(quint32 id)
{
	WeatherView *view = new WeatherView(this, id);
	int result = view->exec();
	if (result == QDialog::Accepted) {
		calendarWidget->update();
	}
	delete view;

	this->editEventRefreshComboBoxes();
}

void MainView::editIntervalType(quint32 id)
{
	IntervalTypeView *view = new IntervalTypeView(this, id);
	int result = view->exec();
	if (result == QDialog::Accepted) {
		calendarWidget->update();
	}
	delete view;
}

void MainView::options()
{
	OptionsView *view = new OptionsView(this);
	int result = view->exec();
	if (result == QDialog::Accepted) {
		calendarWidget->setFirstDayOfWeek(Application::instance()->cfg()->isMondayFirstDayOfWeek() ? Qt::Monday : Qt::Sunday);
		calendarWidget->update();
	}
	delete view;
}

// MAINVIEW

void MainView::systemInformation()
{
	QMessageBox::information(this, tr("System Information"), Application::instance()->systemInformation());
}

void MainView::about()
{
	QMessageBox::about(this, tr("About"), Application::instance()->about());
}

void MainView::updateStatusbar()
{
	if (m_currentEvent) {
		QString message = tr("%1 at %2").arg(m_currentEvent->eventType()->description())
									.arg(m_currentEvent->start().date().toString(Qt::DefaultLocaleShortDate));
		if (!m_currentEvent->name().isEmpty()) {
			message += tr(" - %1").arg(m_currentEvent->name());
		}
		message += tr(" - %1 %2 in %3").arg(Utility::formatDistance(m_currentEvent->distance(), 3))
			.arg(Application::instance()->cfg()->cfgDistanceUnit()->description()).arg(Utility::formatDuration(m_currentEvent->duration()));

		statusbar->showMessage(tr("Event: %1.").arg(message));

		return;
	}

	StatisticsResults::EventsPerDate r = Utility::StatisticsService::instance()->event(calendarWidget->yearShown(), calendarWidget->monthShown());
	if (r.events > 0) {
		QString message = tr("Events: %L1 for a total of %L2 %3.").arg(r.events)
			.arg(Utility::formatDistance(r.distance, 3)).arg(Application::instance()->cfg()->cfgDistanceUnit()->description());
		statusbar->showMessage(message);
		return;
	}

	QString message = tr("No events.");
	statusbar->showMessage(message);
}

void MainView::showDebugOnConsole()
{
//	Application::instance()->objectMap()->free();

	qDebug() << Application::instance()->objectMap()->toString();
}
