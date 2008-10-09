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



void ShoeModel::setShoeMaker(ShoeMaker *value)
{
	if (m_shoeMaker != value) {
		m_shoeMaker = value;
		modified();
	}
}



QList<Objects::BaseObject *> ShoeModel::children() const
{
	QList<Objects::BaseObject *> list;
	list.append(m_shoeMaker);
	return list;
}

QString ShoeModel::toString() const
{
	QString comboObject = static_cast<const ComboObject *>(this)->toString();
	QString shoeMaker = m_shoeMaker ? m_shoeMaker->toString() : "";
	QString s = QString("ShoeModel{%1|%2}").arg(comboObject).arg(shoeMaker);
	return s;
}

QDebug operator<<(QDebug dbg, const ShoeModel &object)
{
	dbg.nospace() << object.toString();
	return dbg.nospace();
}

}
