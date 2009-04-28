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

#include "shoe.h"

#include "shoemodel.h"

namespace Objects {

Shoe::Shoe()
{
	m_shoeModel = NULL;
	m_size = 0.0;
	m_purchaseDate = QDate();
	m_price = 0.0;
	m_initialDistance = 0.0;
	m_retired = false;
	m_notes = "";
}

Shoe::~Shoe()
{
}



void Shoe::setShoeModel(ShoeModel *value)
{
	if (m_shoeModel != value) {
		m_shoeModel = value;
		modified();
	}
}

void Shoe::setSize(qreal value)
{
	if (m_size != value) {
		m_size = value;
		modified();
	}
}

void Shoe::setPurchaseDate(const QDate &value)
{
	if (m_purchaseDate != value) {
		m_purchaseDate = value;
		modified();
	}
}

void Shoe::setPrice(qreal value)
{
	if (m_price != value) {
		m_price = value;
		modified();
	}
}

void Shoe::setInitialDistance(qreal value)
{
	if (m_initialDistance != value) {
		m_initialDistance = value;
		modified();
	}
}

void Shoe::setRetired(bool value)
{
	if (m_retired != value) {
		m_retired = value;
		modified();
	}
}

void Shoe::setNotes(const QString &value)
{
	if (m_notes != value) {
		m_notes = value;
		modified();
	}
}



QList<Objects::BaseObject *> Shoe::children() const
{
	QList<Objects::BaseObject *> list;
	list << m_shoeModel;
	return list;
}

}
