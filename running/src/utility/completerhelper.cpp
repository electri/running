/****************************************************************************

	running - A small program to keep track of your workouts.
	Copyright (C) 2009  Marco Gasparetto (markgabbahey@gmail.com)

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

#include <QSqlQueryModel>
#include "completerhelper.h"

QCompleter *CompleterHelper::completer(const QString &tableName, const QString &columnName, QWidget *parent)
{
	QCompleter *completer = new QCompleter(parent);

	QString queryText = QString("SELECT DISTINCT %1 FROM %2 ORDER BY %1").arg(columnName).arg(tableName);
	QSqlQueryModel *completerModel = new QSqlQueryModel(completer);
	completerModel->setQuery(queryText);

	completer->setModel(completerModel);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);

	return completer;
}
