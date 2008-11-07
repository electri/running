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

#include <QApplication>
#include <QLibraryInfo>
#include <QSqlDatabase>
#include <QStringList>
#include <QDebug>

#include "application.h"

Application* Application::sm_instance = NULL;

Application::Application()
{
	m_objectMap = NULL;
	m_objectRepository = NULL;
	m_cfg = NULL;
}

Application::~Application()
{
}

Application* Application::instance()
{
	if (sm_instance == NULL) {
		sm_instance = new Application();
	}
	return sm_instance;
}



bool Application::init()
{
	bool result = true;

	m_objectRepository = new Services::ObjectRepository();
	result |= m_objectRepository->isActive();

	m_objectMap = new Services::ObjectMap();

	m_cfg = static_cast<Objects::Cfg *>(m_objectMap->getObjectById(Objects::Types::Cfg, 1));

	return result;
}

void Application::close()
{
	delete m_cfg;
	m_cfg = NULL;

	delete m_objectMap;
	m_objectMap = NULL;

	delete m_objectRepository;
	m_objectRepository = NULL;
}

QString Application::about() const
{
	QString message = "<html><head></head><body>\n";
	message += "<h1>" + qApp->applicationName() + "</h1>\n";
	message += qApp->applicationVersion() + "<br><br>\n";
	message += qApp->translate("Application",
		"A simple program for runners based on a calendar and an event list.") + "<br>\n";
	message += "<hr><br>\n";
	message += qApp->translate("Application", "Made with:") + "<br>\n";
	message += "<a href='http://www.mingw.org'>MinGW</a> Compiler<br>\n";
	message += "<a href='http://trolltech.com'>Trolltech Qt</a> Framework<br>\n";
	message += "<a href='http://www.sqlite.org'>SQLite</a> Database engine<br>\n";
	message += "QxtStars widget from <a href='http://libqxt.org'>Qxt library</a> Framework extension<br>\n";
	message += "Icons from <a href='http://www.famfamfam.com/lab/icons/silk'>Silk</a> Icon set<br>\n";
	message += "<hr><br>\n";
	message += "<a href='" + qApp->organizationDomain() + "'>" + qApp->organizationName() + "</a><br>\n";
	message += "</body></html>\n";
	return message;
}

QString Application::systemInformation() const
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
	return message;
}
