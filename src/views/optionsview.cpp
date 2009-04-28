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

#include <QtGui>

#include "optionsview.h"

#include "../application.h"
#include "../objects/cfg.h"
#include "../services/memento.h"
#include "../views/viewhelper.h"

OptionsView::OptionsView(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	m_model = new QStringListModel(this);
	QList<QString> list;
	list << tr("Calendar") << tr("Units of measurement");
	m_model->setStringList(list);

	treeView->setModel(m_model);

	this->refreshComboBoxes();

	m_cfg = static_cast<Objects::Cfg *>(Application::instance()->objectMap()->getObjectById(Objects::Types::Cfg, 1));
	this->setProperties(m_cfg);

	m_memento = new Services::Memento(m_cfg);
}

OptionsView::~OptionsView()
{
	Application::instance()->objectMap()->discardObject(m_cfg);

	delete m_memento;
}



void OptionsView::on_resetPushButton_clicked()
{
	m_memento->revert();
	Objects::Cfg *item = static_cast<Objects::Cfg *>(m_memento->copy());
	this->setProperties(item);
}

void OptionsView::on_savePushButton_clicked()
{
	Objects::Cfg *item = static_cast<Objects::Cfg *>(m_memento->copy());
	this->getProperties(item);

	if (!Application::instance()->objectMap()->saveObject(m_memento->copy())) {
		QMessageBox::critical(this, tr("Edit options"), Application::instance()->objectMap()->lastError());
		return;
	}
	m_memento->submit();

	this->accept();
}

void OptionsView::on_cancelPushButton_clicked()
{
	this->reject();
}



void OptionsView::on_treeView_clicked(const QModelIndex &index)
{
	if (index.isValid()) {
		stackedWidget->setCurrentIndex(index.row());
	}
}



void OptionsView::getProperties(Objects::Cfg *object)
{
	if (object) {
		object->setMondayFirstDayOfWeek(mondayFirstDayOfWeekCheckBox->isChecked());
		object->setCfgDistanceUnit(static_cast<Objects::CfgDistanceUnit *>(ViewHelper::getObjectOnComboBox(cfgDistanceUnitComboBox, Objects::Types::CfgDistanceUnit, object->cfgDistanceUnit())));
		object->setCfgWeightUnit(static_cast<Objects::CfgWeightUnit *>(ViewHelper::getObjectOnComboBox(cfgWeightUnitComboBox, Objects::Types::CfgWeightUnit, object->cfgWeightUnit())));
		object->setCfgTemperatureUnit(static_cast<Objects::CfgTemperatureUnit *>(ViewHelper::getObjectOnComboBox(cfgTemperatureUnitComboBox, Objects::Types::CfgTemperatureUnit, object->cfgTemperatureUnit())));
		object->setCfgCurrencyUnit(static_cast<Objects::CfgCurrencyUnit *>(ViewHelper::getObjectOnComboBox(cfgCurrencyUnitComboBox, Objects::Types::CfgCurrencyUnit, object->cfgCurrencyUnit())));
	}
}

void OptionsView::setProperties(Objects::Cfg *object)
{
	if (object) {
		mondayFirstDayOfWeekCheckBox->setChecked(object->isMondayFirstDayOfWeek());
		ViewHelper::setIndexOnComboBox(cfgDistanceUnitComboBox, object->cfgDistanceUnit());
		ViewHelper::setIndexOnComboBox(cfgWeightUnitComboBox, object->cfgWeightUnit());
		ViewHelper::setIndexOnComboBox(cfgTemperatureUnitComboBox, object->cfgTemperatureUnit());
		ViewHelper::setIndexOnComboBox(cfgCurrencyUnitComboBox, object->cfgCurrencyUnit());
	}
}

void OptionsView::refreshComboBoxes()
{
	ViewHelper::fillComboBox(cfgDistanceUnitComboBox, Objects::Types::CfgDistanceUnit);
	ViewHelper::fillComboBox(cfgWeightUnitComboBox, Objects::Types::CfgWeightUnit);
	ViewHelper::fillComboBox(cfgTemperatureUnitComboBox, Objects::Types::CfgTemperatureUnit);
	ViewHelper::fillComboBox(cfgCurrencyUnitComboBox, Objects::Types::CfgCurrencyUnit);
}
