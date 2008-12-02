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

#ifndef SHOE_H
#define SHOE_H

#include <QDate>

#include "baseobject.h"

namespace Services { class ObjectFactory; }

namespace Objects {

class ShoeModel;

class Shoe : public BaseObject
{
friend class Services::ObjectFactory;

public:
	ShoeModel *shoeModel() const { return m_shoeModel; }
	void setShoeModel(ShoeModel *);

	qreal size() const { return m_size; }
	void setSize(qreal);

	QDate purchaseDate() const { return m_purchaseDate; }
	void setPurchaseDate(const QDate &);

	qreal price() const { return m_price; }
	void setPrice(qreal);

	qreal initialDistance() const { return m_initialDistance; }
	void setInitialDistance(qreal value);

	bool retired() const { return m_retired; }
	void setRetired(bool);

	QString notes() const { return m_notes; }
	void setNotes(const QString &);



	Types::Type type() const { return Types::Shoe; }

	QList<Objects::BaseObject *> children() const;

private:
	Shoe();
	virtual ~Shoe();

	ShoeModel *m_shoeModel;
	qreal m_size;
	QDate m_purchaseDate;
	qreal m_price;
	qreal m_initialDistance;
	bool m_retired;
	QString m_notes;
};

}

#endif
