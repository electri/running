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

#ifndef APPLICATIONSERVICE_H
#define APPLICATIONSERVICE_H

#include "./services/objectmap.h"
#include "./objects/cfg.h"

//#define APP Application::instance()

class Application
{
public:
	virtual ~Application();

	static Application* instance();

	bool init();
	void close();

	QString about() const;
	QString systemInformation() const;
	QString lastError() const;

	Services::ObjectMap *objectMap() const { return m_objectMap; }
	Objects::Cfg *cfg() const { return m_cfg; }

private:
	Application();

	static Application* sm_instance;

	Services::ObjectMap *m_objectMap;
	Objects::Cfg *m_cfg;
};

#endif
