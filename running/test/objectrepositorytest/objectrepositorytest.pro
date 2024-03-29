QT += testlib \
    sql
TEMPLATE = app
TARGET =
CONFIG += qt \
    debug \
    console
DESTDIR = bin/debug
OBJECTS_DIR = obj/debug
DEPENDPATH += . \
    ../..
INCLUDEPATH += . \
    ../..

DEFINES += OBJECTREPOSITORY_DEBUG

# Build
UI_DIR = obj
MOC_DIR = obj
RCC_DIR = obj

# Input
HEADERS += src/objects/baseobject.h \
    src/objects/cfg.h \
    src/objects/comboobject.h \
    src/objects/event.h \
    src/objects/eventtype.h \
    src/objects/interval.h \
    src/objects/intervaltype.h \
    src/objects/shoe.h \
    src/objects/shoemaker.h \
    src/objects/shoemodel.h \
    src/objects/weather.h \
    src/services/database.h \
    src/services/transaction.h \
    src/services/objectfactory.h \
    src/services/objectrepository.h \
    src/services/mappers/baseobjectmapper.h \
    src/services/mappers/cfgmapper.h \
    src/services/mappers/comboobjectmapper.h \
    src/services/mappers/eventmapper.h \
    src/services/mappers/eventtypemapper.h \
    src/services/mappers/intervalmapper.h \
    src/services/mappers/shoemapper.h \
    src/services/mappers/shoemodelmapper.h \
    src/utility/utility.h
SOURCES += src/objects/baseobject.cpp \
    src/objects/cfg.cpp \
    src/objects/comboobject.cpp \
    src/objects/event.cpp \
    src/objects/eventtype.cpp \
    src/objects/interval.cpp \
    src/objects/shoe.cpp \
    src/objects/shoemodel.cpp \
    src/services/database.cpp \
    src/services/transaction.cpp \
    src/services/objectfactory.cpp \
    src/services/objectrepository.cpp \
    src/services/mappers/baseobjectmapper.cpp \
    src/services/mappers/cfgmapper.cpp \
    src/services/mappers/comboobjectmapper.cpp \
    src/services/mappers/eventmapper.cpp \
    src/services/mappers/eventtypemapper.cpp \
    src/services/mappers/intervalmapper.cpp \
    src/services/mappers/shoemapper.cpp \
    src/services/mappers/shoemodelmapper.cpp \
    src/utility/utility.cpp \
    objectrepositorytest.cpp
RESOURCES += resources/application.qrc
