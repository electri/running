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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>

class Settings
{
friend class SettingsView;

public:
	static Settings *instance();

	int toolbarIconSize() const;
	Qt::ToolButtonStyle toolbarToolButtonStyle() const;
	bool isMondayFirstDayOfWeek() const;
	QString currencyUnit() const;
	QString distanceUnit() const;
	QString temperatureUnit() const;
	QString weightUnit() const;
protected:
        /* only instance of this class can destroy itself
           otherwise:
           Settings * instance = Settings::instance();
           delete instance; // not good :-)
           better to use Meyer's singleton
           static Singleton & instance()
           {
               static Singleton instance_;
               return instance_;
           }
         */
        ~Settings();
private:
	Settings();
	static Settings *sm_instance;

	QSettings *m_settings;
};

#endif // SETTINGS_H
