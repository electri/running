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
#include "settings.h"
#include "utility/database.h"
#include "utility/utility.h"
#include "objects/eventfinder.h"
#include "objects/eventgateway.h"
#include "views/settingsview.h"
#include "widgets/calendarwidget/calendarwidgetdelegate.h"

MainView::MainView(QWidget *parent)
	: QMainWindow(parent)
{
	setupUi(this);

#ifdef Q_WS_MAC
	setUnifiedTitleAndToolBarOnMac(true);
#endif

	toolBar->setIconSize(QSize(Settings::instance()->toolbarIconSize(), Settings::instance()->toolbarIconSize()));
	toolBar->setToolButtonStyle(Settings::instance()->toolbarToolButtonStyle());
	calendarWidget->setFirstDayOfWeek(Settings::instance()->isMondayFirstDayOfWeek() ? Qt::Monday : Qt::Sunday);

	connect(actionSystemInformations, SIGNAL(triggered()), this, SLOT(systemInformations()));
	connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(eventWidget, SIGNAL(accept()), this, SLOT(editEventAccepted()));
	connect(eventWidget, SIGNAL(reject()), this, SLOT(editEventRejected()));

	if (Database::init()) {
		calendarWidget->setDelegate(new CalendarWidgetDelegate());

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

void MainView::on_calendarWidget_activated()
{
	EventGateway event;
	if (EventFinder::find(event, calendarWidget->selectedDate())) {
		showEvent();
		eventWidget->setEvent(event);

		QString message = tr("Edit event: %1").arg(event.start().date().toString("d MMMM yyyy"));
		statusbar->showMessage(message);
	} else {
		event.clear();
		event.setStart(QDateTime(calendarWidget->selectedDate(), QTime(0, 0, 0)));

		showEvent();
		eventWidget->setEvent(event);

		QString message = tr("Add event: %1").arg(event.start().date().toString("d MMMM yyyy"));
		statusbar->showMessage(message);
	}
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
	EventGateway event;
	if (EventFinder::find(event, calendarWidget->selectedDate())) {
		QMessageBox::warning(this, tr("Add a new event"), tr("The selected day already has an event."));
	} else {
		event.clear();
		event.setStart(QDateTime(calendarWidget->selectedDate(), QTime(0, 0, 0)));

		showEvent();
		eventWidget->setEvent(event);

		QString message = tr("Add event: %1").arg(event.start().date().toString("d MMMM yyyy"));
		statusbar->showMessage(message);
	}
}

void MainView::editEvent()
{
	EventGateway event;
	if (EventFinder::find(event, calendarWidget->selectedDate())) {
		showEvent();
		eventWidget->setEvent(event);

		QString message = tr("Edit event: %1").arg(event.start().date().toString("d MMMM yyyy"));
		statusbar->showMessage(message);
	} else {
		QMessageBox::warning(this, tr("Edit an event"), tr("The selected day don't have an event."));
	}
}

void MainView::removeEvent()
{
	EventGateway event;
	if (EventFinder::find(event, calendarWidget->selectedDate())) {
		if (QMessageBox::question(this, tr("Remove an event"), tr("Are you sure you want to remove the event: %1?")
				.arg(event.start().date().toString("d MMMM yyyy")),
				QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No) {
			return;
		}

		if (!event.remove()) {
			QMessageBox::critical(this, tr("Remove an event"), event.lastError());
		}

		statisticsWidget->refreshCache();
		calendarWidget->update();
	} else {
		QMessageBox::warning(this, tr("Remove an event"), tr("The selected day don't have an event."));
	}
}

void MainView::editEventAccepted()
{
	statisticsWidget->refreshCache();
	showCalendar();
}

void MainView::editEventRejected()
{
	showCalendar();
}



// SETTINGS

void MainView::settings()
{
	SettingsView *view = new SettingsView(this);
	int result = view->exec();
	if (result == QDialog::Accepted) {
		toolBar->setIconSize(QSize(Settings::instance()->toolbarIconSize(), Settings::instance()->toolbarIconSize()));
		toolBar->setToolButtonStyle(Settings::instance()->toolbarToolButtonStyle());
		eventWidget->applySettings();
		calendarWidget->setFirstDayOfWeek(Settings::instance()->isMondayFirstDayOfWeek() ? Qt::Monday : Qt::Sunday);
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
	message += "Main author: " + qApp->organizationName() + "<br>\n";
	message += "Homepage: <a href='" + qApp->organizationDomain() + "'>" + qApp->organizationDomain() + "</a><br>\n";
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
