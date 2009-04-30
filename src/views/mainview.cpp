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
#include <QtSql>
#include "mainview.h"
#include "utility/database.h"
#include "utility/utility.h"
#include "objects/eventfinder.h"
#include "objects/settingsgateway.h"
//#include "views/intervalview.h"
#include "views/popupviews/runnerinfopopupview.h"
#include "views/popupviews/votepopupview.h"
#include "views/popupviews/weatherinfopopupview.h"
#include "views/tableviews/eventtypeview.h"
//#include "views/shoeview.h"
//#include "views/shoemakerview.h"
//#include "views/shoemodelview.h"
#include "views/settingsview.h"
#include "widgets/calendarwidget/calendarwidgetdelegate.h"
#include "utility/comboboxhelper.h"
#include "utility/completerhelper.h"

MainView::MainView(QWidget *parent)
	: QMainWindow(parent)
{
	setupUi(this);

#ifdef Q_WS_MAC
	setUnifiedTitleAndToolBarOnMac(true);

	toolBar->setIconSize(QSize(32, 32));
	toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
#endif

//	m_intervalview = NULL;
	m_runnerinfopopupview = NULL;
	m_votepopupview = NULL;
	m_weatherinfopopupview = NULL;

	starsSlider->setMinimum(0);
	starsSlider->setMaximum(5);
	starsSlider->setSingleStep(1);
	starsSlider->setPageStep(1);
	QPalette palette = starsSlider->palette();
	palette.setColor(QPalette::Highlight, QColor("yellow"));
	starsSlider->setPalette(palette);

	connect(actionSystemInformations, SIGNAL(triggered()), this, SLOT(systemInformations()));
	connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));

	if (Database::init()) {
		calendarWidget->setDelegate(new CalendarWidgetDelegate());
		calendarWidget->setFirstDayOfWeek(SettingsGateway::instance()->isMondayFirstDayOfWeek() ? Qt::Monday : Qt::Sunday);
		connect(calendarWidget, SIGNAL(activated()), this, SLOT(editEvent()));

		statisticsWidget->refreshCache();

		connect(actionCalendar, SIGNAL(triggered()), this, SLOT(showCalendar()));
		connect(actionStatistics, SIGNAL(triggered()), this, SLOT(showStatistics()));
		connect(actionAdd, SIGNAL(triggered()), this, SLOT(addEvent()));
		connect(actionRemove, SIGNAL(triggered()), this, SLOT(removeEvent()));
		connect(actionEdit, SIGNAL(triggered()), this, SLOT(editEvent()));
		connect(actionSettings, SIGNAL(triggered()), this, SLOT(settings()));
	} else {
		actionCalendar->setEnabled(false);
		actionStatistics->setEnabled(false);
		actionAdd->setEnabled(false);
		actionRemove->setEnabled(false);
		actionEdit->setEnabled(false);
		actionSettings->setEnabled(false);

		QMessageBox::critical(0, tr("Error"), Database::lastError(), QMessageBox::Ok, QMessageBox::NoButton);
	}

	calendarWidget->setSelectedDate(QDate::currentDate());
}

MainView::~MainView()
{
	Database::close();
}



// CALENDAR WIDGET

void MainView::showCalendar()
{
	actionCalendar->setEnabled(true);
	actionStatistics->setEnabled(true);
	actionAdd->setEnabled(true);
	actionEdit->setEnabled(true);
	actionRemove->setEnabled(true);

	stackedWidget->setCurrentIndex(stackedWidget->indexOf(calendarPage));
	on_calendarWidget_selectionChanged();
}

void MainView::on_calendarWidget_selectionChanged()
{
	updateStatusbar();
}



// STATISTICS WIDGET

void MainView::showStatistics()
{
	actionCalendar->setEnabled(true);
	actionStatistics->setEnabled(true);
	actionAdd->setEnabled(false);
	actionRemove->setEnabled(false);
	actionEdit->setEnabled(false);

	stackedWidget->setCurrentIndex(stackedWidget->indexOf(statisticsPage));
}

void MainView::on_statisticsPageEventsPushButton_clicked()
{
	statisticsWidget->setPage(StatisticsPageEventsPerDate);
}

void MainView::on_statisticsPageShoesPushButton_clicked()
{
	statisticsWidget->setPage(StatisticsPageShoes);
}



// EDIT EVENT

void MainView::showEvent()
{
	actionCalendar->setEnabled(false);
	actionStatistics->setEnabled(false);
	actionAdd->setEnabled(false);
	actionEdit->setEnabled(false);
	actionRemove->setEnabled(false);

	stackedWidget->setCurrentIndex(stackedWidget->indexOf(eventPage));
}

void MainView::addEvent()
{
	if (EventFinder::find(m_event, calendarWidget->selectedDate())) {
		QMessageBox::warning(this, tr("Add a new event"), tr("The selected day already has an event."));
	} else {
		showEvent();

		m_event.setStart(QDateTime(calendarWidget->selectedDate(), QTime()));
		editEventBegin();

		QString message = tr("Add event: %1").arg(m_event.start().date().toString("d MMMM yyyy"));
		statusbar->showMessage(message);
	}
}

void MainView::removeEvent()
{
	if (EventFinder::find(m_event, calendarWidget->selectedDate())) {
		if (QMessageBox::question(this, tr("Remove an event"), tr("Are you sure you want to remove the event: %1?")
				.arg(m_event.start().date().toString("d MMMM yyyy")),
				QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No) {
			return;
		}

		if (!m_event.remove()) {
			QMessageBox::critical(this, tr("Remove an event"), m_event.lastError());
		}

		calendarWidget->update();
	} else {
		QMessageBox::warning(this, tr("Remove an event"), tr("The selected day don't have an event."));
	}
}

void MainView::editEvent()
{
	if (EventFinder::find(m_event, calendarWidget->selectedDate())) {
		showEvent();

		editEventBegin();

		QString message = tr("Edit event: %1").arg(m_event.start().date().toString("d MMMM yyyy"));
		statusbar->showMessage(message);
	} else {
		QMessageBox::warning(this, tr("Edit an event"), tr("The selected day don't have an event."));
	}
}

void MainView::editEventBegin()
{
//	m_intervalview = new IntervalView(event, this);
//	m_intervalview->setWindowFlags(Qt::Popup);
	m_runnerinfopopupview = new RunnerInfoPopupView(this);
	m_runnerinfopopupview->setWindowFlags(Qt::Popup);
	m_votepopupview = new VotePopupView(this);
	m_votepopupview->setWindowFlags(Qt::Popup);
	m_weatherinfopopupview = new WeatherInfoPopupView(this);
	m_weatherinfopopupview->setWindowFlags(Qt::Popup);

	nameLineEdit->setCompleter(CompleterHelper::completer("Event", "Name", nameLineEdit));
	descriptionLineEdit->setCompleter(CompleterHelper::completer("Event", "Description", descriptionLineEdit));

	distanceDoubleSpinBox->setSuffix(" " + SettingsGateway::instance()->distanceUnit_description());

	ComboBoxHelper::fillComboBox(eventTypeComboBox, "EventType", false);
	ComboBoxHelper::fillShoesComboBox(shoeComboBox, false);

	editEventSetFields();
}

void MainView::editEventEnd()
{
//	delete m_intervalview;
	delete m_runnerinfopopupview;
	delete m_votepopupview;
	delete m_weatherinfopopupview;

	showCalendar();
}

void MainView::editEventSetFields()
{
	startTimeEdit->setTime(m_event.start().time());
	nameLineEdit->setText(m_event.name());
	descriptionLineEdit->setText(m_event.description());
	distanceDoubleSpinBox->setValue(m_event.distance());
	durationTimeEdit->setTime(m_event.duration());
	notesPlainTextEdit->setPlainText(m_event.notes());
	starsSlider->setValue(m_event.vote());
	m_runnerinfopopupview->weightDoubleSpinBox->setValue(m_event.weight());
	m_votepopupview->qualitySpinBox->setValue(m_event.quality());
	m_votepopupview->effortSpinBox->setValue(m_event.effort());
	m_weatherinfopopupview->temperatureDoubleSpinBox->setValue(m_event.temperature());
	ComboBoxHelper::setSelectedId(eventTypeComboBox, m_event.eventType_id());
	ComboBoxHelper::setSelectedId(shoeComboBox, m_event.shoe_id());
	ComboBoxHelper::setSelectedId(m_weatherinfopopupview->weatherComboBox, m_event.weather_id());

	paceLineEdit->setText(Utility::formatPace(m_event.distance(), m_event.duration()));
}

void MainView::editEventGetFields()
{
	m_event.setStart(QDateTime(calendarWidget->selectedDate(), startTimeEdit->time()));
	m_event.setName(nameLineEdit->text());
	m_event.setDescription(descriptionLineEdit->text());
	m_event.setDistance(distanceDoubleSpinBox->value());
	m_event.setDuration(durationTimeEdit->time());
	m_event.setNotes(notesPlainTextEdit->toPlainText());
	m_event.setVote(starsSlider->value());
	m_event.setQuality(m_votepopupview->qualitySpinBox->value());
	m_event.setEffort(m_votepopupview->effortSpinBox->value());
	m_event.setWeight(m_runnerinfopopupview->weightDoubleSpinBox->value());
	m_event.setTemperature(m_weatherinfopopupview->temperatureDoubleSpinBox->value());
	m_event.setEventType_id(ComboBoxHelper::selectedId(eventTypeComboBox));
	m_event.setShoe_id(ComboBoxHelper::selectedId(shoeComboBox));
	m_event.setWeather_id(ComboBoxHelper::selectedId(m_weatherinfopopupview->weatherComboBox));
}

void MainView::on_resetPushButton_clicked()
{
//	m_intervalview->resetAll();

	editEventSetFields();
}

void MainView::on_savePushButton_clicked()
{
	editEventGetFields();

//	m_intervalview->saveAll();

	if (!m_event.save()) {
		QMessageBox::critical(this, tr("Add/Edit an event"), m_event.lastError());
		return;
	}

	editEventEnd();
}

void MainView::on_cancelPushButton_clicked()
{
	editEventEnd();
}

void MainView::on_eventTypeComboBox_currentIndexChanged(int)
{
//	Services::ObjectMap *session = Application::instance()->objectMap();
//
//	quint32 eventTypeId = ui->eventTypeComboBox->itemData(ui->eventTypeComboBox->currentIndex()).toInt();
//	if (eventTypeId != 0) {
//		Objects::EventType *eventType = static_cast<Objects::EventType *>(session->getObjectById(Objects::Types::EventType, eventTypeId));
//		if (eventType) {
//			ui->intervalsPushButton->setEnabled(eventType->hasIntervals());
//			session->discardObject(eventType);
//		}
//	} else {
//		ui->intervalsPushButton->setEnabled(false);
//	}
}

void MainView::on_distanceDoubleSpinBox_valueChanged(double value)
{
	QTime time = durationTimeEdit->time();
	paceLineEdit->setText(Utility::formatPace(value, time));
}

void MainView::on_durationTimeEdit_timeChanged(const QTime &value)
{
	double distance = distanceDoubleSpinBox->value();
	paceLineEdit->setText(Utility::formatPace(distance, value));
}

void MainView::on_eventTypeToolButton_clicked()
{
	quint32 id = ComboBoxHelper::selectedId(eventTypeComboBox);

	EventTypeView *view = new EventTypeView(this, id);
	int result = view->exec();
	if (result == QDialog::Accepted) {
		calendarWidget->update();
	}
	delete view;

	ComboBoxHelper::fillComboBox(eventTypeComboBox, "EventType", false);
}

//void MainView::on_intervalsPushButton_clicked()
//{
//	m_intervalview->move(ui->intervalsPushButton->mapToGlobal(QPoint(0, (m_intervalview->height() + 10) * -1)));
//	m_intervalview->show();
//}

void MainView::on_runnerInfoPushButton_clicked()
{
	m_runnerinfopopupview->move(runnerInfoPushButton->mapToGlobal(QPoint(0, (m_runnerinfopopupview->height() + 10) * -1)));
	m_runnerinfopopupview->show();
}

void MainView::on_votePushButton_clicked()
{
	m_votepopupview->move(votePushButton->mapToGlobal(QPoint(0, votePushButton->height() + 10)));
	m_votepopupview->show();
}

void MainView::on_weatherInfoPushButton_clicked()
{
	m_weatherinfopopupview->move(weatherInfoPushButton->mapToGlobal(QPoint(0, (m_weatherinfopopupview->height() + 10) * -1)));
	m_weatherinfopopupview->show();
}



// EDIT SHOE

void MainView::on_shoeToolButton_clicked()
{
//	quint32 id = ComboBoxHelper::selectedId(shoeComboBox);
//
//	ShoeView *view = new ShoeView(this, id);
//	int result = view->exec();
//	if (result == QDialog::Accepted) {
//		ui->calendarWidget->update();
//	}
//	delete view;
//
//	ComboBoxHelper::fillShoesComboBox(shoeComboBox, false);
}



// SETTINGS

void MainView::settings()
{
	SettingsView *view = new SettingsView(this);
	int result = view->exec();
	if (result == QDialog::Accepted) {
		calendarWidget->setFirstDayOfWeek(SettingsGateway::instance()->isMondayFirstDayOfWeek() ? Qt::Monday : Qt::Sunday);
		calendarWidget->update();
	}
	delete view;
}



// MAINVIEW

void MainView::systemInformations()
{
	QString message = "<html><head></head><body>\n";
	message += "Qt Library version: " + QString(qVersion()) + ", ";
	message += "build key: " + QLibraryInfo::buildKey() + ", ";
	message += "installed in \"" + QLibraryInfo::location(QLibraryInfo::LibrariesPath) + "\", ";
	QStringList drivers = QSqlDatabase::drivers();
	if (drivers.count() > 0) {
		message += "with following database drivers available: ";
		message += drivers[0];
		for (int i = 1; i < drivers.count(); ++i) message += ", " + drivers[i];
		message += ".<br>\n";
	} else {
		message += "with no database drivers available.<br>\n";
	}
	message += "</body></html>\n";
	QMessageBox::information(this, tr("System Information"), message);
}

void MainView::about()
{
	QString message = "<html><head></head><body>\n";
	message += "<h1>" + qApp->applicationName() + "</h1>\n";
	message += qApp->applicationVersion() + "<br><br>\n";
	message += tr("A simple program for runners based on a calendar and an event list.") + "<br>\n";
	message += "<hr><br>\n";
	message += tr("Made with:") + "<br>\n";
	message += "<a href='http://gcc.gnu.org'>GCC</a> / <a href='http://www.mingw.org'>MinGW</a> Compiler<br>\n";
	message += "Nokia <a href='http://www.qtsoftware.com'>Qt Framework</a> and Qt Creator IDE<br>\n";
	message += "<a href='http://www.sqlite.org'>SQLite</a> Database engine<br>\n";
	message += "QxtStars widget from <a href='http://libqxt.org'>Qxt library</a> Framework extension<br>\n";
	message += "Icons from: ";
	message += "<a href='http://www.famfamfam.com'>Silk</a>, ";
	message += "<a href='http://www.iconspedia.com'>Weby</a> and ";
	message += "<a href='http://www.kde.org'>Oxygen</a> Icon Sets<br>\n";
	message += "<hr><br>\n";
	message += "<a href='" + qApp->organizationDomain() + "'>" + qApp->organizationName() + "</a><br>\n";
	message += "</body></html>\n";
	QMessageBox::about(this, tr("About"), message);
}

void MainView::updateStatusbar()
{
	EventGateway event;
	if (EventFinder::find(event, calendarWidget->selectedDate())) {
		QString message = tr("%1 at %2")
				.arg(event.eventType_description())
				.arg(event.start().date().toString(Qt::DefaultLocaleShortDate));
		if (!event.name().isEmpty()) {
			message += tr(" - %1").arg(event.name());
		}
		message += tr(" - %1 in %3")
				.arg(Utility::formatDistance(event.distance(), 3))
				.arg(Utility::formatDuration(event.duration()));
		statusbar->showMessage(tr("Event: %1").arg(message));
	} else {
		QString message = tr("No event");
		statusbar->showMessage(message);
	}
}
