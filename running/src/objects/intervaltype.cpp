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

#include <QtDebug>

#include "intervaltype.h"

namespace Objects {

IntervalType::IntervalType()
{
}

IntervalType::~IntervalType()
{
}



QString IntervalType::toString() const
{
	QString comboObject = static_cast<const ComboObject *>(this)->toString();
	QString s = QString("IntervalType{%1}").arg(comboObject);
	return s;
}

QDebug operator<<(QDebug dbg, const IntervalType &object)
{
	dbg.nospace() << object.toString();
	return dbg.nospace();
}

}
