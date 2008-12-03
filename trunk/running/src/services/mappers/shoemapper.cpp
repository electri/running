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

#include <QtSql>

#include "shoemapper.h"

#include "../../objects/shoe.h"
#include "../../objects/shoemaker.h"
#include "../../objects/shoemodel.h"

namespace Mappers {

ShoeMapper::ShoeMapper(Services::ObjectRepository *repository)
	: BaseObjectMapper(repository)
{
	m_table = "Shoe";
	m_order = "Id";
	m_columnList = "ShoeModelId, Size, PurchaseDate, Price, InitialDistance, Retired, Notes";
	m_valueList = ":shoemodelid, :size, :purchasedate, :price, :initialdistance, :retired, :notes";
}



void ShoeMapper::setValuesFromFields(Objects::BaseObject *object, QSqlQuery &query)
{
	Objects::Shoe *shoe = static_cast<Objects::Shoe *>(object);

	Objects::ShoeModel *shoeModel = static_cast<Objects::ShoeModel *>(m_repository->getChild(Objects::Types::ShoeModel, query.record().value("ShoeModelId").toInt()));

	shoe->setShoeModel(shoeModel ? shoeModel : 0);
	shoe->setSize(query.record().value("Size").toDouble());
	shoe->setPurchaseDate(query.record().value("PurchaseDate").toDate());
	shoe->setPrice(query.record().value("Price").toDouble());
	shoe->setInitialDistance(query.record().value("InitialDistance").toDouble());
	shoe->setRetired(query.record().value("Retired").toBool());
	shoe->setNotes(query.record().value("Notes").toString());
}

void ShoeMapper::setFieldsFromValues(Objects::BaseObject *object, QSqlQuery &query)
{
	Objects::Shoe *shoe = static_cast<Objects::Shoe *>(object);

	query.bindValue(":shoemodelid", shoe->shoeModel() ? shoe->shoeModel()->id() : 0);
	query.bindValue(":size", shoe->size());
	query.bindValue(":purchasedate", shoe->purchaseDate());
	query.bindValue(":price", shoe->price());
	query.bindValue(":initialdistance", shoe->initialDistance());
	query.bindValue(":retired", shoe->retired());
	query.bindValue(":notes", shoe->notes());
}

}
