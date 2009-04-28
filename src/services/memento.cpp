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

#include "memento.h"

#include "../services/objectfactory.h"

namespace Services {

Memento::Memento(Objects::BaseObject *original)
{
	m_original = original;

	if (m_original) {
		m_copy = ObjectFactory::createObject(m_original->type());
		ObjectFactory::copyObject(m_original, m_copy);
	} else {
		m_copy = NULL;
	}
}

Memento::~Memento()
{
	if (m_copy) {
		delete m_copy;
	}
}

void Memento::submit()
{
	if (m_original && m_copy) {
		ObjectFactory::copyObject(m_copy, m_original);
	}
}

void Memento::revert()
{
	if (m_original && m_copy) {
		ObjectFactory::copyObject(m_original, m_copy);
	}
}

}
