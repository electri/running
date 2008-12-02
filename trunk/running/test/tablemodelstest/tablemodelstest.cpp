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

#include <QtTest/QtTest>
#include <QtSql>

#include "../../src/objects/event.h"
#include "../../src/objects/eventtype.h"
#include "../../src/objects/interval.h"
#include "../../src/objects/intervaltype.h"
#include "../../src/objects/shoemaker.h"
#include "../../src/application.h"
#include "../../src/models/comboobjecttablemodel.h"
#include "../../src/models/intervaltablemodel.h"

class TableModelsTest: public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();
	void cleanupTestCase();

	void test_ComboObjectTableModel();
	void test_IntervalTableModel();

private:
	Application *app;
};

void TableModelsTest::initTestCase()
{
	Q_INIT_RESOURCE(application);

	QLocale::setDefault(QLocale::QLocale("en_US"));
	QDir::setCurrent(QApplication::applicationDirPath());

	app = Application::instance();
	QVERIFY2(app->init(), QTest::toString(app->lastError()));

	QSqlQuery query;
	QVERIFY(query.exec("DELETE FROM Interval"));
	QVERIFY(query.exec("DELETE FROM Event"));
	QVERIFY(query.exec("DELETE FROM Shoe"));
	QVERIFY(query.exec("DELETE FROM ShoeModel"));
	QVERIFY(query.exec("DELETE FROM ShoeMaker"));
}

void TableModelsTest::cleanupTestCase()
{
}

void TableModelsTest::test_ComboObjectTableModel()
{
	Services::ObjectMap *map = Application::instance()->objectMap();

	QList<Objects::BaseObject *> list = map->getAllObjects(Objects::Types::ShoeMaker);
	QCOMPARE(list.count(), 0);

	ComboObjectTableModel *model = new ComboObjectTableModel(Objects::Types::ShoeMaker, 0);
	QCOMPARE(model->rowCount(), 0);

	// INSERT ROWS
	model->insertRows(0, 3);
	model->setData(model->index(0, 1), "MakerTest1");
	model->setData(model->index(1, 1), "MakerTest2");
	model->setData(model->index(2, 1), "MakerTest3");
	QVERIFY(model->submitAll());
	QCOMPARE(model->rowCount(), 3);

	list = map->getAllObjects(Objects::Types::ShoeMaker);
	QCOMPARE(list.count(), 3);
	QCOMPARE(static_cast<Objects::ShoeMaker *>(list[2])->description(), QString("MakerTest3"));
	map->discardObjects(list);

	// REMOVE ROWS
	model->removeRows(2, 1); // MakerTest3
	QVERIFY(model->submitAll());
	QCOMPARE(model->rowCount(), 2);

	list = map->getAllObjects(Objects::Types::ShoeMaker);
	QCOMPARE(list.count(), 2);
	QCOMPARE(static_cast<Objects::ShoeMaker *>(list[0])->description(), QString("MakerTest1"));
	QCOMPARE(static_cast<Objects::ShoeMaker *>(list[1])->description(), QString("MakerTest2"));
	map->discardObjects(list);

	// REVERT CHANGES
	model->insertRows(2, 2);
	model->setData(model->index(2, 1), "MakerTest4");
	model->setData(model->index(3, 1), "MakerTest5");
	model->removeRows(0, 1); // MakerTest1
	model->setData(model->index(0, 1), "MakerTest2u");
	model->setData(model->index(2, 1), "MakerTest5u");
	model->revertAll();
	QCOMPARE(model->rowCount(), 2);

	list = map->getAllObjects(Objects::Types::ShoeMaker);
	QCOMPARE(list.count(), 2);
	QCOMPARE(static_cast<Objects::ShoeMaker *>(list[0])->description(), QString("MakerTest1"));
	QCOMPARE(static_cast<Objects::ShoeMaker *>(list[1])->description(), QString("MakerTest2"));
	map->discardObjects(list);

	// SUBMIT CHANGES
	model->insertRows(2, 2);
	model->setData(model->index(2, 1), "MakerTest4");
	model->setData(model->index(3, 1), "MakerTest5");
	model->removeRows(0, 1); // MakerTest1
	model->setData(model->index(0, 1), "MakerTest2u");
	model->setData(model->index(2, 1), "MakerTest5u");
	QVERIFY(model->submitAll());
	QCOMPARE(model->rowCount(), 3);

	list = map->getAllObjects(Objects::Types::ShoeMaker);
	QCOMPARE(list.count(), 3);
	QCOMPARE(static_cast<Objects::ShoeMaker *>(list[0])->description(), QString("MakerTest2u"));
	QCOMPARE(static_cast<Objects::ShoeMaker *>(list[1])->description(), QString("MakerTest4"));
	QCOMPARE(static_cast<Objects::ShoeMaker *>(list[2])->description(), QString("MakerTest5u"));
	map->discardObjects(list);
}

void TableModelsTest::test_IntervalTableModel()
{
	Services::ObjectMap *map = Application::instance()->objectMap();

	Objects::IntervalType *intervalType1 = static_cast<Objects::IntervalType *>(map->getObjectById(Objects::Types::IntervalType, 1));
	QVERIFY(intervalType1 != NULL);
	Objects::Interval *interval1 = static_cast<Objects::Interval *>(map->createObject(Objects::Types::Interval));
	interval1->setIntervalType(intervalType1);
	interval1->setDistance(2.0);
	interval1->setDuration(QTime(0, 5, 0));
	map->registerObject(intervalType1);
	Objects::Interval *interval2 = static_cast<Objects::Interval *>(map->createObject(Objects::Types::Interval));
	interval2->setIntervalType(intervalType1);
	interval2->setDistance(1.0);
	interval2->setDuration(QTime(0, 3, 0));
	Objects::EventType *eventType1 = static_cast<Objects::EventType *>(map->getObjectById(Objects::Types::EventType, 1));
	QVERIFY(eventType1 != NULL);
	Objects::Event *event1 = static_cast<Objects::Event *>(map->createObject(Objects::Types::Event));
	event1->setStart(QDateTime(QDate(2008, 1, 1), QTime(8, 0, 0)));
	event1->setName("Training");
	event1->setDescription("Fake event for tests.");
	event1->setEventType(eventType1);
	event1->setDistance(10.0);
	event1->setDuration(QTime(1, 0, 0));
	event1->setNotes("Notes Notes Notes Notes Notes");
	event1->addInterval(interval1);
	event1->addInterval(interval2);
	QVERIFY2(map->saveObject(event1), QTest::toString(map->lastError()));

	QList<Objects::BaseObject *> list = map->getAllObjects(Objects::Types::Interval);
	QCOMPARE(list.count(), 2);

	IntervalTableModel *model = new IntervalTableModel(event1, 0);
	QCOMPARE(model->rowCount(), 2);

	// INSERT ROWS
	model->insertRows(2, 1);
	model->setData(model->index(2, 1), intervalType1->id());
	model->setData(model->index(2, 2), 3.0);
	model->setData(model->index(2, 3), QTime(0, 8, 0));
	QVERIFY(model->submitAll());
	QCOMPARE(model->rowCount(), 3);

	QVERIFY2(map->saveObject(event1), QTest::toString(map->lastError()));

	list = map->getAllObjects(Objects::Types::Interval);
	QCOMPARE(list.count(), 3);
	QCOMPARE(static_cast<Objects::Interval *>(list[2])->distance(), 3.0);
	map->discardObjects(list);

//	// REMOVE ROWS
//	model->removeRows(2, 1); // MakerTest3
//	QVERIFY(model->submitAll());
//	QCOMPARE(model->rowCount(), 2);
//
//	list = map->getAllObjects(Objects::Types::ShoeMaker);
//	QCOMPARE(list.count(), 2);
//	QCOMPARE(static_cast<Objects::ShoeMaker *>(list[0])->description(), QString("MakerTest1"));
//	QCOMPARE(static_cast<Objects::ShoeMaker *>(list[1])->description(), QString("MakerTest2"));
//	map->discardObjects(list);
//
//	// REVERT CHANGES
//	model->insertRows(2, 2);
//	model->setData(model->index(2, 1), "MakerTest4");
//	model->setData(model->index(3, 1), "MakerTest5");
//	model->removeRows(0, 1); // MakerTest1
//	model->setData(model->index(0, 1), "MakerTest2u");
//	model->setData(model->index(2, 1), "MakerTest5u");
//	model->revertAll();
//	QCOMPARE(model->rowCount(), 2);
//
//	list = map->getAllObjects(Objects::Types::ShoeMaker);
//	QCOMPARE(list.count(), 2);
//	QCOMPARE(static_cast<Objects::ShoeMaker *>(list[0])->description(), QString("MakerTest1"));
//	QCOMPARE(static_cast<Objects::ShoeMaker *>(list[1])->description(), QString("MakerTest2"));
//	map->discardObjects(list);
//
//	// SUBMIT CHANGES
//	model->insertRows(2, 2);
//	model->setData(model->index(2, 1), "MakerTest4");
//	model->setData(model->index(3, 1), "MakerTest5");
//	model->removeRows(0, 1); // MakerTest1
//	model->setData(model->index(0, 1), "MakerTest2u");
//	model->setData(model->index(2, 1), "MakerTest5u");
//	QVERIFY(model->submitAll());
//	QCOMPARE(model->rowCount(), 3);
//
//	list = map->getAllObjects(Objects::Types::ShoeMaker);
//	QCOMPARE(list.count(), 3);
//	QCOMPARE(static_cast<Objects::ShoeMaker *>(list[0])->description(), QString("MakerTest2u"));
//	QCOMPARE(static_cast<Objects::ShoeMaker *>(list[1])->description(), QString("MakerTest4"));
//	QCOMPARE(static_cast<Objects::ShoeMaker *>(list[2])->description(), QString("MakerTest5u"));
//	map->discardObjects(list);
}

QTEST_MAIN(TableModelsTest)
#include "obj/tablemodelstest.moc"
