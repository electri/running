#ifndef CALENDARMODEL_H
#define CALENDARMODEL_H

#include <QAbstractTableModel>

class CalendarItem;

class CalendarModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	CalendarModel(QObject *parent = 0);
	~CalendarModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

//	int row(quint32 id) const;
	int row(const QDate &date) const;
//	CalendarItem calendarItem(const QDate &date) const;
//	void setCalendarItem(const CalendarItem &newItem);

private:
	QList<QString> m_head;
	QList<CalendarItem> m_list;
};

#endif // CALENDARMODEL_H
