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

#include "shoemodelmapper.h"

#include "../../objects/shoemaker.h"
#include "../../objects/shoemodel.h"

#include "../objectmap.h"
//#include "../objectfactory.h"
//#include "../objectrepository.h"

namespace Mappers {

ShoeModelMapper::ShoeModelMapper()
	: ComboObjectMapper("ShoeModel")
{
	m_order = "Description";
	m_columnList = "ShoeMakerId, Description";
	m_valueList = ":shoemakerid, :description";
}



void ShoeModelMapper::get(Objects::BaseObject *object, QSqlQuery &query)
{
	Objects::ShoeModel *shoeModel = static_cast<Objects::ShoeModel *>(object);

	Objects::ShoeMaker *shoeMaker = static_cast<Objects::ShoeMaker *>(
		Services::ObjectMap::instance()->getObjectById(shoeModel->shoeMaker(), Objects::Types::ShoeMaker, query.record().value("ShoeMakerId").toInt()));
//	Objects::ShoeMaker *shoeMaker = static_cast<Objects::ShoeMaker *>(
//										Services::ObjectFactory::instance()->createObject(Objects::Types::ShoeMaker));
//	Services::ObjectRepository::instance()->selectObject(shoeMaker, query.record().value("ShoeMakerId").toInt());

	shoeModel->setShoeMaker(shoeMaker);
	shoeModel->setDescription(query.record().value("Description").toString());
}

void ShoeModelMapper::set(Objects::BaseObject *object, QSqlQuery &query)
{
	Objects::ShoeModel *shoeModel = static_cast<Objects::ShoeModel *>(object);

	query.bindValue(":shoemakerid", shoeModel->shoeMaker() ? shoeModel->shoeMaker()->id() : 0);
	query.bindValue(":description", shoeModel->description());
}

}
