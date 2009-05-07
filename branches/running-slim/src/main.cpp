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

#include <QApplication>
#include "views/mainview.h"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(application);

	QApplication app(argc, argv);
	app.setApplicationName("Running");
	app.setApplicationVersion("pre 0.3 (Slim Branch)");
	app.setOrganizationName("Marco Gasparetto");
	app.setOrganizationDomain("http://code.google.com/p/running");

	MainView view;
	view.show();

	return app.exec();
}
