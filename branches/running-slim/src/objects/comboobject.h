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

#ifndef COMBOOBJECT_H
#define COMBOOBJECT_H

#include "baseobject.h"

namespace Mappers { class ComboObjectMapper; }

namespace Objects {

class ComboObject : public BaseObject
{
friend class Mappers::ComboObjectMapper;

public:
	QString description() const { return m_description; }
	void setDescription(const QString &);

	virtual Types::Type type() const = 0;

	QString toString() const;

protected:
	ComboObject();
	virtual ~ComboObject();

	QString m_description;
};

QDebug operator<<(QDebug, const ComboObject &);

}

#endif
