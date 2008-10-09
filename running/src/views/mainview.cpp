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

#include "../objects/event.h"
#include "../objects/eventtype.h"
#include "../objects/shoe.h"
#include "../objects/shoemaker.h"
#include "../objects/shoemodel.h"
#include "../objects/weather.h"
#include "../services/objectmap.h"
#include "../services/memento.h"
#include "../services/objectrepository.h"
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
#include "../widgets/qxtstars/qxtstars.h"

MainView::MainView(QWidget *parent)
	: QMainWindow(parent)
{
	setupUi(this);

	this->showCalendar();

	starsSlider->setMinimum(0);
	starsSlider->setMaximum(5);
	starsSlider->setSingleStep(1);
	starsSlider->setPageStep(1);
	QPalette palette = starsSlider->palette();
	palette.setColor(QPalette::Highlight, QColor("yellow"));
	starsSlider->setPalette(palette);

	connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));

	if (Services::ObjectRepository::instance()->isActive()) {
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

		QMessageBox::critical(0, tr("Error"), tr("Unable to open or read the database file."),
																	QMessageBox::Ok, QMessageBox::NoButton);
	}

//	calendarWidget->setSelectedDate(QDate(2006, 11, 1));

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(showDebugOnConsole()));
	timer->start(3000);
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
	Services::ObjectMap *session = Services::ObjectMap::instance();

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
		m_currentEvent = static_cast<Objects::Event *>(Services::ObjectMap::instance()->createObject(Objects::Types::Event));
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

		if (!Services::ObjectMap::instance()->deleteObject(m_currentEvent)) {
			QMessageBox::critical(this, tr("Remove an event"),
							tr("An error has occoured during saving modifications in the database."));
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
		QMessageBox::warning(this, tr("Edit an event"),
						tr("The selected day don't have an event."));
	}
}

void MainView::editEventBegin(Objects::Event *object)
{
		m_votepopupview = new VotePopupView(this);
		m_votepopupview->setWindowFlags(Qt::Popup);

		m_intervalview = new IntervalView(object, this);
		m_intervalview->setWindowFlags(Qt::Popup);

		m_runnerinfopopupview = new RunnerInfoPopupView(this);
		m_runnerinfopopupview->setWindowFlags(Qt::Popup);

		m_weatherinfopopupview = new WeatherInfoPopupView(this);
		m_weatherinfopopupview->setWindowFlags(Qt::Popup);

		editEventRefreshComboBoxes();

		m_memento = new Services::Memento(object);

		Objects::Event *event = static_cast<Objects::Event *>(m_memento->copy());
		editEventSetProperties(event);

		stackedWidget->setCurrentIndex(stackedWidget->indexOf(eventPage));

		actionShowCalendar->setEnabled(false);
		actionShowStatistics->setEnabled(false);
		actionAdd->setEnabled(false);
		actionEdit->setEnabled(false);
		actionRemove->setEnabled(false);
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
		Services::ObjectMap *session = Services::ObjectMap::instance();

		object->setStart(QDateTime(object->start().date(), startTimeEdit->time()));
		object->setName(nameLineEdit->text());
		object->setDescription(descriptionLineEdit->text());
		quint32 eventTypeId = eventTypeComboBox->itemData(eventTypeComboBox->currentIndex()).toInt();
		if ((object->eventType()) && (object->eventType()->id() != eventTypeId)) {
			session->discardObject(object->eventType());
			object->setEventType(NULL);
		}
		if ((!object->eventType()) && (eventTypeId != 0)) {
			object->setEventType(static_cast<Objects::EventType *>(session->getObjectById(Objects::Types::EventType, eventTypeId)));
		}
		object->setDistance(distanceDoubleSpinBox->value());
		object->setDuration(durationTimeEdit->time());
		object->setNotes(notesPlainTextEdit->toPlainText());
		quint32 shoeId = shoeComboBox->itemData(shoeComboBox->currentIndex()).toInt();
		if ((object->shoe()) && (object->shoe()->id() != shoeId)) {
			session->discardObject(object->shoe());
			object->setShoe(NULL);
		}
		if ((!object->shoe()) && (shoeId != 0)) {
			object->setShoe(static_cast<Objects::Shoe *>(session->getObjectById(Objects::Types::Shoe, shoeId)));
		}
		object->setVote(starsSlider->value());
		object->setQuality(m_votepopupview->qualitySpinBox->value());
		object->setEffort(m_votepopupview->effortSpinBox->value());
		object->setWeight(m_runnerinfopopupview->weightDoubleSpinBox->value());
		quint32 weatherId = m_weatherinfopopupview->weatherComboBox->itemData(m_weatherinfopopupview->weatherComboBox->currentIndex()).toInt();
		if ((object->weather()) && (object->weather()->id() != weatherId)) {
			session->discardObject(object->weather());
			object->setWeather(NULL);
		}
		if ((!object->weather()) && (weatherId != 0)) {
			object->setWeather(static_cast<Objects::Weather *>(session->getObjectById(Objects::Types::Weather, weatherId)));
		}
		object->setTemperature(m_weatherinfopopupview->temperatureDoubleSpinBox->value());
	}
}

void MainView::editEventSetProperties(Objects::Event *object)
{
	if (object) {
		startTimeEdit->setTime(object->start().time());
		nameLineEdit->setText(object->name());
		descriptionLineEdit->setText(object->description());
		if (object->eventType()) {
			eventTypeComboBox->setCurrentIndex(eventTypeComboBox->findData(object->eventType()->id()));
		}
		distanceDoubleSpinBox->setValue(object->distance());
		durationTimeEdit->setTime(object->duration());
		notesPlainTextEdit->setPlainText(object->notes());
		if (object->shoe()) {
			shoeComboBox->setCurrentIndex(shoeComboBox->findData(object->shoe()->id()));
		}
		starsSlider->setValue(object->vote());
		m_votepopupview->qualitySpinBox->setValue(object->quality());
		m_votepopupview->effortSpinBox->setValue(object->effort());
		m_runnerinfopopupview->weightDoubleSpinBox->setValue(object->weight());
		if (object->weather()) {
			m_weatherinfopopupview->weatherComboBox->setCurrentIndex(m_weatherinfopopupview->weatherComboBox->findData(object->weather()->id()));
		}
		m_weatherinfopopupview->temperatureDoubleSpinBox->setValue(object->temperature());
	}
}

void MainView::editEventRefreshComboBoxes()
{
	Services::ObjectMap *session = Services::ObjectMap::instance();
	{
		quint32 id = eventTypeComboBox->itemData(eventTypeComboBox->currentIndex()).toInt();
		eventTypeComboBox->clear();
		QList<Objects::BaseObject *> list = session->getAllObjects(Objects::Types::EventType);
		foreach (Objects::BaseObject *object, list) {
			Objects::EventType *item = static_cast<Objects::EventType *>(object);
			eventTypeComboBox->addItem(item->description(), item->id());
		}
		session->discardObjects(list);
		eventTypeComboBox->setCurrentIndex(eventTypeComboBox->findData(id));
	}
	{
		quint32 id = shoeComboBox->itemData(shoeComboBox->currentIndex()).toInt();
		shoeComboBox->clear();
		QList<Objects::BaseObject *> list = session->getAllObjects(Objects::Types::Shoe);
		foreach (Objects::BaseObject *object, list) {
			Objects::Shoe *item = static_cast<Objects::Shoe *>(object);
			shoeComboBox->addItem(QString("%1 %2").arg(item->shoeModel()->shoeMaker()->description())
													.arg(item->shoeModel()->description()), item->id());
		}
		session->discardObjects(list);
		shoeComboBox->setCurrentIndex(shoeComboBox->findData(id));
	}
	if (m_weatherinfopopupview) {
		m_weatherinfopopupview->refreshComboBoxes();
	}
}

void MainView::on_resetPushButton_clicked()
{
	m_intervalview->resetAll();

	m_memento->revert();
	Objects::Event *item = static_cast<Objects::Event *>(m_memento->copy());
	editEventSetProperties(item);
}

void MainView::on_savePushButton_clicked()
{
	Objects::Event *event = static_cast<Objects::Event *>(m_memento->copy());
	editEventGetProperties(event);

	if (!Services::ObjectMap::instance()->saveObject(m_memento->copy())) {
		QMessageBox::critical(this, tr("Add/Edit an event"),
						tr("An error has occoured during saving modifications in the database.") +
						"\n\n" + Services::ObjectRepository::instance()->lastError());
		return;
	}
	m_memento->submit();

	if (!m_intervalview->saveAll()) {
		QMessageBox::critical(this, tr("Add/Edit an event"),
						tr("An error has occoured during saving modifications in the database.") +
						"\n\n" + Services::ObjectRepository::instance()->lastError());
		return;
	}

	editEventEnd();
}

void MainView::on_cancelPushButton_clicked()
{
	editEventEnd();
}

void MainView::on_eventTypeComboBox_currentIndexChanged(int index)
{
	Services::ObjectMap *session = Services::ObjectMap::instance();

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
	QTime paceTime = Utility::paceTime(value, time);
	double paceSpeed = Utility::paceSpeed(value, time);
	paceLineEdit->setText(tr("%1 min/km or %2 km/h")
			.arg(Utility::formatDuration(paceTime)).arg(Utility::formatDistance(paceSpeed, 2)));
}

void MainView::on_durationTimeEdit_timeChanged(const QTime &value)
{
	double distance = distanceDoubleSpinBox->value();
	QTime paceTime = Utility::paceTime(distance, value);
	double paceSpeed = Utility::paceSpeed(distance, value);
	paceLineEdit->setText(tr("%1 min/km or %2 km/h")
			.arg(Utility::formatDuration(paceTime)).arg(Utility::formatDistance(paceSpeed, 2)));
}

void MainView::on_eventTypeToolButton_clicked()
{
	quint32 id = eventTypeComboBox->itemData(eventTypeComboBox->currentIndex()).toInt();
	this->editEventType(id);
}

void MainView::on_votePushButton_clicked()
{
	m_votepopupview->move(votePushButton->mapToGlobal(QPoint(0, m_votepopupview->height() + 10)));
	m_votepopupview->show();
}

void MainView::on_intervalsPushButton_clicked()
{
	m_intervalview->move(intervalsPushButton->mapToGlobal(QPoint(0, (m_intervalview->height() + 10) * -1)));
	m_intervalview->show();

//	IntervalView *view = new IntervalView(m_currentEvent, this);
//	int result = view->exec();
//	if (result == QDialog::Accepted) {
//		calendarWidget->update();
//	}
//	delete view;
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

// MAINVIEW

void MainView::about()
{
	QString message = "<html><head></head><body>\n";
	message += "<h1>" + qApp->applicationName() + "</h1>\n";
//	message += qApp->applicationVersion() + "<br>\n";
	message += tr("A simple program for runners based on a calendar and an event list.") + "<br>\n";
	message += "<hr><br>\n";
	message += tr("Made with:") + "<br>\n";
	message += "<a href='http://www.mingw.org'>MinGW</a> Compiler<br>\n";
	message += "<a href='http://www.eclipse.org'>Eclipse</a> Integrated development environment<br>\n";
	message += "<a href='http://trolltech.com'>Trolltech Qt</a> Framework<br>\n";
	message += "<a href='http://www.sqlite.org'>SQLite</a> Database engine<br>\n";
	message += "QxtStars widget from <a href='http://libqxt.org'>Qxt library</a> Framework extension<br>\n";
	message += "Icons from <a href='http://www.famfamfam.com/lab/icons/silk'>Silk</a> Icon set<br>\n";
	message += "<hr><br>\n";
	message += "<a href='" + qApp->organizationDomain() + "'>" + qApp->organizationName() + "</a><br>\n";
	message += "</body></html>\n";

	QMessageBox::about(this, tr("About"), message);
}

void MainView::updateStatusbar()
{
	if (m_currentEvent) {
		QString message = tr("%1 at %2").arg(m_currentEvent->eventType()->description())
									.arg(m_currentEvent->start().date().toString(Qt::DefaultLocaleShortDate));
		if (!m_currentEvent->name().isEmpty()) {
			message += tr(" - %1").arg(m_currentEvent->name());
		}
		message += tr(" - %1 km in %2").arg(Utility::formatDistance(m_currentEvent->distance(), 3))
									.arg(Utility::formatDuration(m_currentEvent->duration()));

		statusbar->showMessage(tr("Event: %1.").arg(message));

		return;
	}

	StatisticsResults::EventsPerDate r = Utility::StatisticsService::instance()->event(calendarWidget->yearShown(), calendarWidget->monthShown());
	if (r.events > 0) {
		QString message = tr("Events: %L1 for a total of %L2 km.").arg(r.events).arg(Utility::formatDistance(r.distance, 3));
		statusbar->showMessage(message);
		return;
	}

	QString message = tr("No events.");
	statusbar->showMessage(message);
}

void MainView::showDebugOnConsole()
{
//	Services::ObjectMap::instance()->free();

//	qDebug() << Services::ObjectMap::instance()->toString();
}
