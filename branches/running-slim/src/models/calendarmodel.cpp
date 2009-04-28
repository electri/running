#include <QDebug>

#include "calendarmodel.h"
#include "calendaritem.h"

CalendarModel::CalendarModel(QObject *parent)
		: QAbstractTableModel(parent)
{
	m_head << "id" << "text1" << "text2" << "date1" << "value1" << "time1";
	m_list << CalendarItem(0, "prova", "descrizione item 1", QDate(2009, 2, 8), 2.0, QTime(0, 5, 20, 0));
	m_list << CalendarItem(1, "prova", "descrizione item 2", QDate(2009, 2, 10), 15.8, QTime(1, 5, 20, 0));
	m_list << CalendarItem(2, "prova", "descrizione item 3", QDate(2009, 2, 15), 5.0, QTime(0, 35, 20, 0));
	m_list << CalendarItem(3, "prova", "descrizione item 4", QDate(2009, 2, 20), 5.8, QTime(0, 25, 20, 0));
	m_list << CalendarItem(4, "prova", "descrizione item 5", QDate(2009, 3, 20), 5.8, QTime(0, 20, 0, 0));
}

CalendarModel::~CalendarModel()
{
}

int CalendarModel::rowCount(const QModelIndex &) const
{
	return m_list.count();
}

int CalendarModel::columnCount(const QModelIndex &) const
{
	return m_head.count();
}

QVariant CalendarModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) return QVariant();

	if ((index.column() >= m_head.size()) || (index.row() >= m_list.size())) return QVariant();

	if ((role != Qt::DisplayRole) && (role != Qt::EditRole)) return QVariant();

	const CalendarItem &item = m_list.at(index.row());
	QVariant result;
	switch (index.column()) {
		case 0: result = item.id; break;
		case 1: result = item.text1; break;
		case 2: result = item.text2; break;
		case 3: result = item.date1; break;
		case 4: result = item.value1; break;
		case 5: result = item.time1; break;
	}
	return result;
}

QVariant CalendarModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole) return QVariant();

	if (orientation == Qt::Horizontal) {
		return m_head.at(section);
	} else {
		return QString("%1").arg(section + 1);
	}
}

Qt::ItemFlags CalendarModel::flags(const QModelIndex &index) const
{
	if (!index.isValid()) return Qt::ItemIsEnabled;

	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool CalendarModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
//	qDebug() << "CalendarModel::setData(index:" << index.row() << "," << index.column() << ", value:" << value;

	if (index.isValid() && role == Qt::EditRole) {
		CalendarItem item = m_list.takeAt(index.row());
		switch (index.column()) {
			case 0:		break;
			case 1:		item.text1 = value.toString();		break;
			case 2:		item.text2 = value.toString();		break;
			case 3:		item.date1 = value.toDate();		break;
			case 4:		item.value1 = value.toDouble();		break;
			case 5:		item.time1 = value.toTime();		break;
		}
		m_list.insert(index.row(), item);

		emit dataChanged(index, index);

		return true;
	}
	return false;
}

//int CalendarModel::row(quint32 id) const
//{
//	for (int i = 0; i < m_list.count(); ++i) {
//		CalendarItem item = m_list.at(i);
//		if (item.id == id) {
//			return i;
//		}
//	}
//	return -1;
//}

int CalendarModel::row(const QDate &date) const
{
	for (int i = 0; i < m_list.count(); ++i) {
		CalendarItem item = m_list.at(i);
		if (item.date1 == date) {
			return i;
		}
	}
	return -1;
}

//CalendarItem CalendarModel::calendarItem(const QDate &date) const
//{
//	foreach (const CalendarItem &item, m_list) {
//		if (item.date1 == date) {
//			return item;
//		}
//	}
//	return CalendarItem();
//}
//
//void CalendarModel::setCalendarItem(const CalendarItem &newItem)
//{
//	if (newItem.isValid()) {
//		int row = this->row(newItem.id);
//		QModelIndex index = this->index(row, 0);
//		setData(index.sibling(row, 1), newItem.text1);
//		setData(index.sibling(row, 2), newItem.text2);
//		setData(index.sibling(row, 3), newItem.date1);
//		setData(index.sibling(row, 4), newItem.value1);
//		setData(index.sibling(row, 5), newItem.time1);
//	}
//}
