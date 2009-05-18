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
#include "settings.h"
#include "utility/comboboxhelper.h"

SettingsView::SettingsView(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	m_model = new QStringListModel(this);
	QList<QString> list;
	list << tr("General") << tr("Calendar") << tr("Units of measurement");
	m_model->setStringList(list);

	treeView->setModel(m_model);

	toolbarIconSizeComboBox->addItem("16", 16);
	toolbarIconSizeComboBox->addItem("24", 24);
	toolbarIconSizeComboBox->addItem("32", 32);
	toolbarIconSizeComboBox->addItem("48", 48);
	toolbarIconSizeComboBox->addItem("64", 64);

	toolbarToolButtonStyleComboBox->addItem(tr("Only display the icon"), 0);
	toolbarToolButtonStyleComboBox->addItem(tr("Only display the text"), 1);
	toolbarToolButtonStyleComboBox->addItem(tr("The text appears beside the icon"), 2);
	toolbarToolButtonStyleComboBox->addItem(tr("The text appears under the icon"), 3);

	ComboBoxHelper::fillComboBox(currencyUnitComboBox, "CfgCurrencyUnit", false);
	ComboBoxHelper::fillComboBox(distanceUnitComboBox, "CfgDistanceUnit", false);
	ComboBoxHelper::fillComboBox(temperatureUnitComboBox, "CfgTemperatureUnit", false);
	ComboBoxHelper::fillComboBox(weightUnitComboBox, "CfgWeightUnit", false);

	_refresh();
}

SettingsView::~SettingsView()
{
}

void SettingsView::on_resetPushButton_clicked()
{
	_refresh();
}

void SettingsView::on_savePushButton_clicked()
{
	QSettings *settings = Settings::instance()->m_settings;

	settings->setValue("General/Toolbar Icon Size", ComboBoxHelper::selected(toolbarIconSizeComboBox));
	settings->setValue("General/Toolbar Tool Button Style", ComboBoxHelper::selected(toolbarToolButtonStyleComboBox));
	settings->setValue("Calendar/Monday is first day of week", mondayFirstDayOfWeekCheckBox->isChecked());
	settings->setValue("Units of measurement/Currency Unit", currencyUnitComboBox->currentText());
	settings->setValue("Units of measurement/Distance Unit", distanceUnitComboBox->currentText());
	settings->setValue("Units of measurement/Temperature Unit", temperatureUnitComboBox->currentText());
	settings->setValue("Units of measurement/Weight Unit", weightUnitComboBox->currentText());

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

void SettingsView::_refresh()
{
	ComboBoxHelper::setSelected(toolbarToolButtonStyleComboBox, Settings::instance()->toolbarToolButtonStyle());
	ComboBoxHelper::setSelected(toolbarIconSizeComboBox, Settings::instance()->toolbarIconSize());
	mondayFirstDayOfWeekCheckBox->setChecked(Settings::instance()->isMondayFirstDayOfWeek());
	ComboBoxHelper::setSelected(currencyUnitComboBox, Settings::instance()->currencyUnit());
	ComboBoxHelper::setSelected(distanceUnitComboBox, Settings::instance()->distanceUnit());
	ComboBoxHelper::setSelected(temperatureUnitComboBox, Settings::instance()->temperatureUnit());
	ComboBoxHelper::setSelected(weightUnitComboBox, Settings::instance()->weightUnit());
}
