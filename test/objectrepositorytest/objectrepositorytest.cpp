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
#include "../../src/services/database.h"
#include "../../src/services/objectrepository.h"

class ObjectRepositoryTest: public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();
	void cleanupTestCase();

	void test_CreateSaveErase();
	void test_Collections();
	void test_Transaction();

private:
	Services::Database *database;
	Services::ObjectRepository *repository;
};

void ObjectRepositoryTest::initTestCase()
{
	Q_INIT_RESOURCE(application);

	QLocale::setDefault(QLocale::QLocale("en_US"));
	QDir::setCurrent(QApplication::applicationDirPath());

	database = new Services::Database();
	QVERIFY2(database->isActive(), QTest::toString(database->lastError()));

	repository = new Services::ObjectRepository();

	QSqlQuery query;
	QVERIFY(query.exec("DELETE FROM Interval"));
	QVERIFY(query.exec("DELETE FROM Event"));
	QVERIFY(query.exec("DELETE FROM Shoe"));
	QVERIFY(query.exec("DELETE FROM ShoeModel"));
	QVERIFY(query.exec("DELETE FROM ShoeMaker"));
}

void ObjectRepositoryTest::cleanupTestCase()
{
	if (repository) {
		delete repository;
	}
	if (database) {
		delete database;
	}
}

void ObjectRepositoryTest::test_CreateSaveErase()
{
	Objects::IntervalType *intervalType1 = static_cast<Objects::IntervalType *>(repository->createObject(Objects::Types::IntervalType));
	QVERIFY(repository->loadObject(intervalType1, 1));

	Objects::Interval *interval1 = static_cast<Objects::Interval *>(repository->createObject(Objects::Types::Interval));
	interval1->setIntervalType(intervalType1);
	interval1->setDistance(1.0);
	interval1->setDuration(QTime(0, 5, 0));

	Objects::Interval *interval2 = static_cast<Objects::Interval *>(repository->createObject(Objects::Types::Interval));
	interval2->setIntervalType(intervalType1);
	interval2->setDistance(2.0);
	interval2->setDuration(QTime(0, 4, 0));

	Objects::EventType *eventType1 = static_cast<Objects::EventType *>(repository->createObject(Objects::Types::EventType));
	QVERIFY(repository->loadObject(eventType1, 1));

	Objects::Event *event1 = static_cast<Objects::Event *>(repository->createObject(Objects::Types::Event));
	event1->setStart(QDateTime(QDate(2008, 1, 1), QTime(8, 0, 0)));
	event1->setName("Training");
	event1->setDescription("Fake event for tests.");
	event1->setEventType(eventType1);
	event1->setDistance(10.0);
	event1->setDuration(QTime(1, 0, 0));
	event1->setNotes("Notes Notes Notes Notes Notes");
	event1->addInterval(interval1);
	event1->addInterval(interval2);

	// SAVE (INSERT)
	QVERIFY2(repository->saveObject(event1), QTest::toString(repository->lastError()));

	QList<quint32> eventIdList = repository->getIdList(Objects::Types::Event);
	QCOMPARE(eventIdList.count(), 1);

	QList<quint32> intervalIdList = repository->getIdList(Objects::Types::Interval);
	QCOMPARE(intervalIdList.count(), 2);

	// SAVE (UPDATE)
	interval2->setDistance(1.0);

	QVERIFY2(repository->saveObject(event1), QTest::toString(repository->lastError()));

	Objects::Interval *interval2u = static_cast<Objects::Interval *>(repository->createObject(Objects::Types::Interval));
	QVERIFY(repository->loadObject(interval2u, interval2->id()));

	QCOMPARE(interval2u->distance(), 1.0);

	// LOAD
	Objects::Event *event1u = static_cast<Objects::Event *>(repository->createObject(Objects::Types::Event));
	QVERIFY(repository->loadObject(event1u, event1->id()));

	QCOMPARE(event1u->eventType()->id(), 1u);

	// ERASE
	QVERIFY(repository->eraseObject(event1));

	eventIdList = repository->getIdList(Objects::Types::Event);
	QCOMPARE(eventIdList.count(), 0);

	intervalIdList = repository->getIdList(Objects::Types::Interval);
	QCOMPARE(intervalIdList.count(), 0);
}

void ObjectRepositoryTest::test_Collections()
{
	Objects::EventType *eventType1 = static_cast<Objects::EventType *>(repository->createObject(Objects::Types::EventType));
	QVERIFY(repository->loadObject(eventType1, 1));

	Objects::Event *event1 = static_cast<Objects::Event *>(repository->createObject(Objects::Types::Event));
	event1->setStart(QDateTime(QDate(2008, 1, 1), QTime(8, 0, 0)));
	event1->setName("Training");
	event1->setEventType(eventType1);

	QVERIFY2(repository->saveObject(event1), QTest::toString(repository->lastError()));

	// ADD ITEM TO COLLECTION
	Objects::IntervalType *intervalType1 = static_cast<Objects::IntervalType *>(repository->createObject(Objects::Types::IntervalType));
	QVERIFY(repository->loadObject(intervalType1, 1));

	Objects::Interval *interval1 = static_cast<Objects::Interval *>(repository->createObject(Objects::Types::Interval));
	interval1->setIntervalType(intervalType1);

	event1->addInterval(interval1);
	QVERIFY2(repository->saveObject(event1), QTest::toString(repository->lastError()));

	Objects::Event *event1u = static_cast<Objects::Event *>(repository->createObject(Objects::Types::Event));
	QVERIFY(repository->loadObject(event1u, event1->id()));
	QCOMPARE(event1u->eventType()->id(), 1u);

	QCOMPARE(event1u->intervals().count(), 1);

	QList<quint32> intervalList = repository->getIdList(Objects::Types::Interval);
	QCOMPARE(intervalList.count(), 1);

	// REMOVE ITEM FROM COLLECTION
	event1->removeInterval(interval1);
	QVERIFY2(repository->saveObject(event1), QTest::toString(repository->lastError()));

	event1u = static_cast<Objects::Event *>(repository->createObject(Objects::Types::Event));
	QVERIFY(repository->loadObject(event1u, event1->id()));
	QCOMPARE(event1u->eventType()->id(), 1u);

	QCOMPARE(event1u->intervals().count(), 0);

	intervalList = repository->getIdList(Objects::Types::Interval);
	QCOMPARE(intervalList.count(), 0);

	// CLEANUP
	QVERIFY2(repository->eraseObject(event1), QTest::toString(repository->lastError()));
}

void ObjectRepositoryTest::test_Transaction()
{
	QList<quint32> eventIdList = repository->getIdList(Objects::Types::Event);
	QCOMPARE(eventIdList.count(), 0);
	QList<quint32> intervalIdList = repository->getIdList(Objects::Types::Interval);
	QCOMPARE(intervalIdList.count(), 0);

	Objects::Interval *interval1 = static_cast<Objects::Interval *>(repository->createObject(Objects::Types::Interval));
	interval1->setDistance(1.0);
	interval1->setDuration(QTime(0, 5, 0));

	Objects::EventType *eventType1 = static_cast<Objects::EventType *>(repository->createObject(Objects::Types::EventType));
	QVERIFY(repository->loadObject(eventType1, 1));

	Objects::Event *event1 = static_cast<Objects::Event *>(repository->createObject(Objects::Types::Event));
	event1->setStart(QDateTime(QDate(2008, 1, 1), QTime(8, 0, 0)));
	event1->setName("Training");
	event1->setDescription("Fake event for tests.");
	event1->setEventType(eventType1);
	event1->addInterval(interval1);

	// SAVE (INSERT)
	QVERIFY(repository->saveObject(event1) == false);

	eventIdList = repository->getIdList(Objects::Types::Event);
	QCOMPARE(eventIdList.count(), 0);
	intervalIdList = repository->getIdList(Objects::Types::Interval);
	QCOMPARE(intervalIdList.count(), 0);
}

QTEST_MAIN(ObjectRepositoryTest)
#include "obj/objectrepositorytest.moc"
