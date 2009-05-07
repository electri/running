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

#ifndef ABSTRACTTABLEVIEW_H
#define ABSTRACTTABLEVIEW_H

#include <QDialog>
#include <QModelIndex>

class QTableView;
class QSqlTableModel;

class AbstractTableView : public QDialog
{
	Q_OBJECT

public:
	AbstractTableView(QWidget *parent = 0);
	virtual ~AbstractTableView();

protected slots:
	virtual void currentRowChanged(const QModelIndex &current, const QModelIndex &previous) = 0;

	void on_addPushButton_clicked();
	void on_removePushButton_clicked();
	void on_resetPushButton_clicked();
	void on_savePushButton_clicked();
	void on_cancelPushButton_clicked();

private slots:
	void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

protected:
	virtual void setControlsEnabled(bool value);
	void refresh(quint32 id = 0);

	QSqlTableModel *m_model;
	QTableView *m_tableView;
	bool m_pendingDataChanges;
};

#endif // ABSTRACTTABLEVIEW_H
