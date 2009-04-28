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
#include "../../src/services/objectmap.h"

class ObjectMapTest: public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();
	void cleanupTestCase();

	void test_CreateSaveErase();
	void test_Collections();

private:
	Services::ObjectMap *map;
};

void ObjectMapTest::initTestCase()
{
	Q_INIT_RESOURCE(application);

	QLocale::setDefault(QLocale::QLocale("en_US"));
	QDir::setCurrent(QApplication::applicationDirPath());

	map = new Services::ObjectMap();
	QVERIFY2(map->isActive(), QTest::toString(map->lastError()));

	QSqlQuery query;
	QVERIFY(query.exec("DELETE FROM Interval"));
	QVERIFY(query.exec("DELETE FROM Event"));
	QVERIFY(query.exec("DELETE FROM Shoe"));
	QVERIFY(query.exec("DELETE FROM ShoeModel"));
	QVERIFY(query.exec("DELETE FROM ShoeMaker"));
}

void ObjectMapTest::cleanupTestCase()
{
	if (map) {
		delete map;
	}
}

void ObjectMapTest::test_CreateSaveErase()
{
	Objects::IntervalType *intervalType1 = static_cast<Objects::IntervalType *>(map->getObjectById(Objects::Types::IntervalType, 1));
	QVERIFY(intervalType1 != NULL);

	Objects::Interval *interval1 = static_cast<Objects::Interval *>(map->createObject(Objects::Types::Interval));
	interval1->setIntervalType(intervalType1);
	interval1->setDistance(1.0);
	interval1->setDuration(QTime(0, 5, 0));

	map->registerObject(intervalType1);

	Objects::Interval *interval2 = static_cast<Objects::Interval *>(map->createObject(Objects::Types::Interval));
	interval2->setIntervalType(intervalType1);
	interval2->setDistance(2.0);
	interval2->setDuration(QTime(0, 4, 0));

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

	QCOMPARE(map->objCount(), 5);
	QCOMPARE(map->refCount(Objects::Types::Event), 1);
	QCOMPARE(map->refCount(Objects::Types::EventType), 1);
	QCOMPARE(map->refCount(Objects::Types::Interval), 2);
	QCOMPARE(map->refCount(Objects::Types::IntervalType), 2);

	// SAVE (INSERT)
	QVERIFY2(map->saveObject(event1), QTest::toString(map->lastError()));

	QList<Objects::BaseObject *> eventList = map->getAllObjects(Objects::Types::Event);
	QCOMPARE(eventList.count(), 1);
	map->discardObjects(eventList);

	QList<Objects::BaseObject *> intervalList = map->getAllObjects(Objects::Types::Interval);
	QCOMPARE(intervalList.count(), 2);
	map->discardObjects(intervalList);

	QCOMPARE(map->objCount(), 5);
	QCOMPARE(map->refCount(Objects::Types::Event), 1);
	QCOMPARE(map->refCount(Objects::Types::EventType), 1);
	QCOMPARE(map->refCount(Objects::Types::Interval), 2);
	QCOMPARE(map->refCount(Objects::Types::IntervalType), 2);

	// SAVE (UPDATE)
	interval2->setDistance(1.0);
	QVERIFY2(map->saveObject(event1), QTest::toString(map->lastError()));

	Objects::Interval *interval2u = static_cast<Objects::Interval *>(map->getObjectById(Objects::Types::Interval, interval2->id()));
	QVERIFY(interval2u != NULL);
	QCOMPARE(interval2u->distance(), 1.0);
	map->discardObject(interval2u);

	QCOMPARE(map->objCount(), 5);
	QCOMPARE(map->refCount(Objects::Types::Event), 1);
	QCOMPARE(map->refCount(Objects::Types::EventType), 1);
	QCOMPARE(map->refCount(Objects::Types::Interval), 2);
	QCOMPARE(map->refCount(Objects::Types::IntervalType), 2);

	// LOAD
	Objects::Event *event1u = static_cast<Objects::Event *>(map->getObjectById(Objects::Types::Event, event1->id()));
	QVERIFY(event1u != NULL);
	QCOMPARE(event1u->eventType()->id(), 1u);

	QCOMPARE(map->objCount(), 5);
	QCOMPARE(map->refCount(Objects::Types::Event), 2);
	QCOMPARE(map->refCount(Objects::Types::EventType), 2);
	QCOMPARE(map->refCount(Objects::Types::Interval), 4);
	QCOMPARE(map->refCount(Objects::Types::IntervalType), 4);

	map->discardObject(event1u);

	QCOMPARE(map->objCount(), 5);
	QCOMPARE(map->refCount(Objects::Types::Event), 1);
	QCOMPARE(map->refCount(Objects::Types::EventType), 1);
	QCOMPARE(map->refCount(Objects::Types::Interval), 2);
	QCOMPARE(map->refCount(Objects::Types::IntervalType), 2);

	// ERASE
	QVERIFY(map->eraseObject(event1));

	QCOMPARE(map->objCount(), 5);

	eventList = map->getAllObjects(Objects::Types::Event);
	QCOMPARE(eventList.count(), 0);

	intervalList = map->getAllObjects(Objects::Types::Interval);
	QCOMPARE(intervalList.count(), 0);

	QCOMPARE(map->objCount(), 5);

	// FREE
	map->discardObject(event1);

	QCOMPARE(map->objCount(), 5);
	QCOMPARE(map->refCount(Objects::Types::Event), 0);
	QCOMPARE(map->refCount(Objects::Types::EventType), 0);
	QCOMPARE(map->refCount(Objects::Types::Interval), 0);
	QCOMPARE(map->refCount(Objects::Types::IntervalType), 0);

	map->free();

	QCOMPARE(map->objCount(), 0);
}

void ObjectMapTest::test_Collections()
{
	Objects::EventType *eventType1 = static_cast<Objects::EventType *>(map->getObjectById(Objects::Types::EventType, 1));
	QVERIFY(eventType1 != NULL);

	Objects::Event *event1 = static_cast<Objects::Event *>(map->createObject(Objects::Types::Event));
	event1->setStart(QDateTime(QDate(2008, 1, 1), QTime(8, 0, 0)));
	event1->setName("Training");
	event1->setEventType(eventType1);

	QVERIFY2(map->saveObject(event1), QTest::toString(map->lastError()));

	QCOMPARE(map->objCount(), 2);
	QCOMPARE(map->refCount(Objects::Types::Event), 1);
	QCOMPARE(map->refCount(Objects::Types::EventType), 1);
	QCOMPARE(map->refCount(Objects::Types::Interval), 0);
	QCOMPARE(map->refCount(Objects::Types::IntervalType), 0);

	// ADD ITEM TO COLLECTION
	Objects::IntervalType *intervalType1 = static_cast<Objects::IntervalType *>(map->getObjectById(Objects::Types::IntervalType, 1));
	QVERIFY(intervalType1 != NULL);

	Objects::Interval *interval1 = static_cast<Objects::Interval *>(map->createObject(Objects::Types::Interval));
	interval1->setIntervalType(intervalType1);

	event1->addInterval(interval1);
	QVERIFY2(map->saveObject(event1), QTest::toString(map->lastError()));

	QCOMPARE(map->objCount(), 4);
	QCOMPARE(map->refCount(Objects::Types::Event), 1);
	QCOMPARE(map->refCount(Objects::Types::EventType), 1);
	QCOMPARE(map->refCount(Objects::Types::Interval), 1);
	QCOMPARE(map->refCount(Objects::Types::IntervalType), 1);

	Objects::Event *event1u = static_cast<Objects::Event *>(map->getObjectById(Objects::Types::Event, event1->id()));
	QVERIFY(event1u != NULL);
	QCOMPARE(event1u->eventType()->id(), 1u);

	QCOMPARE(event1u->intervals().count(), 1);

	QCOMPARE(map->objCount(), 4);
	QCOMPARE(map->refCount(Objects::Types::Event), 2);
	QCOMPARE(map->refCount(Objects::Types::EventType), 2);
	QCOMPARE(map->refCount(Objects::Types::Interval), 2);
	QCOMPARE(map->refCount(Objects::Types::IntervalType), 2);

	map->discardObject(event1u);

	QList<Objects::BaseObject *> intervalList = map->getAllObjects(Objects::Types::Interval);
	QCOMPARE(intervalList.count(), 1);
	map->discardObjects(intervalList);

	// REMOVE ITEM FROM COLLECTION
	event1->removeInterval(interval1);
	QVERIFY2(map->saveObject(event1), QTest::toString(map->lastError()));
	map->discardObject(interval1);

	QCOMPARE(map->objCount(), 4);
	QCOMPARE(map->refCount(Objects::Types::Event), 1);
	QCOMPARE(map->refCount(Objects::Types::EventType), 1);
	QCOMPARE(map->refCount(Objects::Types::Interval), 0);
	QCOMPARE(map->refCount(Objects::Types::IntervalType), 0);

	event1u = static_cast<Objects::Event *>(map->getObjectById(Objects::Types::Event, event1->id()));
	QVERIFY(event1u != NULL);
	QCOMPARE(event1u->eventType()->id(), 1u);

	QCOMPARE(event1u->intervals().count(), 0);

	QCOMPARE(map->objCount(), 4);
	QCOMPARE(map->refCount(Objects::Types::Event), 2);
	QCOMPARE(map->refCount(Objects::Types::EventType), 2);
	QCOMPARE(map->refCount(Objects::Types::Interval), 0);
	QCOMPARE(map->refCount(Objects::Types::IntervalType), 0);

	map->discardObject(event1u);

	intervalList = map->getAllObjects(Objects::Types::Interval);
	QCOMPARE(intervalList.count(), 0);
	map->discardObjects(intervalList);

	// CLEANUP
	QVERIFY2(map->eraseObject(event1), QTest::toString(map->lastError()));
	map->discardObject(event1);
	QCOMPARE(map->objCount(), 4);

	map->free();
	QCOMPARE(map->objCount(), 0);
}

QTEST_MAIN(ObjectMapTest)
#include "obj/objectmaptest.moc"
