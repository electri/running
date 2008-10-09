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

#include <QDebug>

#include "baseobject.h"

namespace Objects {

BaseObject::BaseObject()
{
	m_id = quint32();
	m_state = States::Created;
}

void BaseObject::modified()
{
	if (m_state == States::Selected) {
		m_state = States::Modified;
	}
}

Objects::BaseObject *BaseObject::parent() const
{
	return NULL;
}

void BaseObject::setParent(Objects::BaseObject *)
{
}

QList<Objects::BaseObject *> BaseObject::children() const
{
	return QList<Objects::BaseObject *>();
}

QString BaseObject::toString() const
{
	QString state;
	switch (m_state) {
		case States::Selected: state = "Sel"; break;
		case States::Created:  state = "Cre";  break;
		case States::Modified: state = "Mod"; break;
		case States::Deleted:  state = "Del";  break;
	}
	QString s = QString("BaseObject{%1|%2}").arg(m_id).arg(state);
	return s;
}

QDebug operator<<(QDebug dbg, const BaseObject &object)
{
	dbg.nospace() << object.toString();
	return dbg.space();
}

}
