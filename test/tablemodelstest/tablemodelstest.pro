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
    src/services/memento.h \
    src/services/objectfactory.h \
    src/services/objectmap.h \
    src/services/objectmapinterface.h \
    src/services/objectrepository.h \
    src/services/mappers/baseobjectmapper.h \
    src/services/mappers/cfgmapper.h \
    src/services/mappers/comboobjectmapper.h \
    src/services/mappers/eventmapper.h \
    src/services/mappers/eventtypemapper.h \
    src/services/mappers/intervalmapper.h \
    src/services/mappers/shoemapper.h \
    src/services/mappers/shoemodelmapper.h \
    src/utility/utility.h \
    src/models/baseobjecttablemodel.h \
    src/models/comboobjecttablemodel.h \
    src/models/childobjecttablemodel.h \
    src/models/eventtypetablemodel.h \
    src/models/shoemodeltablemodel.h \
    src/models/shoetablemodel.h \
    src/models/intervaltablemodel.h \
    src/application.h
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
    src/services/memento.cpp \
    src/services/objectfactory.cpp \
    src/services/objectmap.cpp \
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
    src/models/baseobjecttablemodel.cpp \
    src/models/comboobjecttablemodel.cpp \
    src/models/childobjecttablemodel.cpp \
    src/models/eventtypetablemodel.cpp \
    src/models/shoemodeltablemodel.cpp \
    src/models/shoetablemodel.cpp \
    src/models/intervaltablemodel.cpp \
    src/application.cpp \
    tablemodelstest.cpp
RESOURCES += resources/application.qrc
