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

#include <QtGui>
#include <QtSql>
#include "settingsview.h"
#include "objects/settingsgateway.h"
#include "utility/comboboxhelper.h"

SettingsView::SettingsView(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	m_model = new QStringListModel(this);
	QList<QString> list;
	list << tr("Calendar") << tr("Units of measurement");
	m_model->setStringList(list);

	treeView->setModel(m_model);

	ComboBoxHelper::fillComboBox(currencyUnitComboBox, "CfgCurrencyUnit", false);
	ComboBoxHelper::fillComboBox(distanceUnitComboBox, "CfgDistanceUnit", false);
	ComboBoxHelper::fillComboBox(temperatureUnitComboBox, "CfgTemperatureUnit", false);
	ComboBoxHelper::fillComboBox(weightUnitComboBox, "CfgWeightUnit", false);

	setFields();
}

SettingsView::~SettingsView()
{
}

void SettingsView::on_resetPushButton_clicked()
{
	setFields();
}

void SettingsView::on_savePushButton_clicked()
{
	getFields();

	if (!SettingsGateway::instance()->update()) {
		QMessageBox::critical(this, tr("Edit settings"), SettingsGateway::instance()->lastError());
		return;
	}

	accept();
}

void SettingsView::on_cancelPushButton_clicked()
{
	reject();
}

void SettingsView::on_treeView_clicked(const QModelIndex &index)
{
	if (index.isValid()) {
		stackedWidget->setCurrentIndex(index.row());
	}
}

void SettingsView::setFields()
{
	mondayFirstDayOfWeekCheckBox->setChecked(SettingsGateway::instance()->isMondayFirstDayOfWeek());
	ComboBoxHelper::setSelectedId(currencyUnitComboBox, SettingsGateway::instance()->currencyUnit_id());
	ComboBoxHelper::setSelectedId(distanceUnitComboBox, SettingsGateway::instance()->distanceUnit_id());
	ComboBoxHelper::setSelectedId(temperatureUnitComboBox, SettingsGateway::instance()->temperatureUnit_id());
	ComboBoxHelper::setSelectedId(weightUnitComboBox, SettingsGateway::instance()->weightUnit_id());
}

void SettingsView::getFields()
{
	SettingsGateway::instance()->setMondayFirstDayOfWeek(mondayFirstDayOfWeekCheckBox->isChecked());
	SettingsGateway::instance()->setCurrencyUnitId(ComboBoxHelper::selectedId(currencyUnitComboBox));
	SettingsGateway::instance()->setDistanceUnitId(ComboBoxHelper::selectedId(distanceUnitComboBox));
	SettingsGateway::instance()->setTemperatureUnitId(ComboBoxHelper::selectedId(temperatureUnitComboBox));
	SettingsGateway::instance()->setWeightUnitId(ComboBoxHelper::selectedId(weightUnitComboBox));
}
