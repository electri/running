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

#ifndef DISTANCESTYLEDITEMDELEGATE_H
#define DISTANCESTYLEDITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QLocale>

class DistanceStyledItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	DistanceStyledItemDelegate(QObject *parent = 0);
	DistanceStyledItemDelegate(quint8 precision, QObject *parent = 0);
	DistanceStyledItemDelegate(quint8 precision, const QString &prefix, const QString &suffix, QObject *parent = 0);

	QString displayText(const QVariant &value, const QLocale &locale) const;

private:
	uint m_precision;
	QString m_prefix;
	QString m_suffix;
};

#endif // DISTANCESTYLEDITEMDELEGATE_H
