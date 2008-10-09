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

#ifndef SHOEMODEL_H
#define SHOEMODEL_H

#include "comboobject.h"

namespace Services { class ObjectFactory; }

namespace Objects {

class ShoeMaker;

class ShoeModel : public ComboObject
{
friend class Services::ObjectFactory;

public:
	ShoeMaker *shoeMaker() const { return m_shoeMaker; }
	void setShoeMaker(ShoeMaker *);

	Types::Type type() const { return Types::ShoeModel; }

	QList<Objects::BaseObject *> children() const;

	QString toString() const;

private:
	ShoeModel();
	virtual ~ShoeModel();

	ShoeMaker *m_shoeMaker;
};

QDebug operator<<(QDebug, const ShoeModel &);

}

#endif
