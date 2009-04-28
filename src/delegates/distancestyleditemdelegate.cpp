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

#include "distancestyleditemdelegate.h"
#include "utility/utility.h"

DistanceStyledItemDelegate::DistanceStyledItemDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
	m_precision = 0;
	m_prefix = "";
	m_suffix = "";
}

DistanceStyledItemDelegate::DistanceStyledItemDelegate(quint8 precision, QObject *parent)
	: QStyledItemDelegate(parent)
{
	m_precision = precision;
	m_prefix = "";
	m_suffix = "";
}

DistanceStyledItemDelegate::DistanceStyledItemDelegate(quint8 precision, const QString &prefix, const QString &suffix, QObject *parent)
	: QStyledItemDelegate(parent)
{
	m_precision = precision;
	m_prefix = prefix;
	m_suffix = suffix;
}

QString DistanceStyledItemDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
	Q_UNUSED(locale);

	if (value.isNull()) return "";

	QString s = "";

	bool ok = false;
	double d = value.toDouble(&ok);
	if (ok) {
		s = Utility::formatDistance(d, m_precision);
	}

	if (s != "") {
		if (m_prefix != "") {
			s = m_prefix + " " + s;
		}
		if (m_suffix != "") {
			s = s + " " + m_suffix;
		}
	}

	return s;
}
