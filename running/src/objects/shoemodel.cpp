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

#include "shoemodel.h"

#include "shoemaker.h"

namespace Objects {

ShoeModel::ShoeModel()
{
	m_shoeMaker = NULL;
}

ShoeModel::~ShoeModel()
{
}



void ShoeModel::setShoeMaker(BaseObject *value)
{
	ShoeMaker *object = static_cast<ShoeMaker *>(value);

	if (m_shoeMaker != object) {
		m_shoeMaker = object;
		modified();
	}
}



QList<Objects::BaseObject *> ShoeModel::children() const
{
	QList<Objects::BaseObject *> list;
	list << m_shoeMaker;
	return list;
}

}
