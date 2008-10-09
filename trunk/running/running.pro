QT += core \
	gui \
	sql

TEMPLATE = app

CONFIG += qt \
	debug_and_release
CONFIG(debug, debug|release) {
	TARGET = running
	CONFIG += debug \
		console
	DESTDIR = bin/debug
	OBJECTS_DIR = obj/debug
} else {
	TARGET = running
	DESTDIR = bin/release
	OBJECTS_DIR = obj/release
}

DEPENDPATH += src \
	resources
INCLUDEPATH += src \
	resources

# Build
UI_DIR = obj
MOC_DIR = obj
RCC_DIR = obj

# Input
HEADERS += src/objects/baseobject.h \
    src/objects/comboobject.h \
    src/objects/event.h \
    src/objects/eventtype.h \
    src/objects/shoe.h \
    src/objects/shoemaker.h \
    src/objects/shoemodel.h \
    src/objects/weather.h \
    src/objects/interval.h \
    src/objects/intervaltype.h \
    src/services/memento.h \
    src/services/objectfactory.h \
    src/services/objectmap.h \
    src/services/objectrepository.h \
    src/services/mappers/baseobjectmapper.h \
    src/services/mappers/comboobjectmapper.h \
    src/services/mappers/eventmapper.h \
    src/services/mappers/eventtypemapper.h \
    src/services/mappers/shoemapper.h \
    src/services/mappers/shoemodelmapper.h \
    src/services/mappers/intervalmapper.h \
    src/delegates/booleanimageitemdelegate.h \
    src/delegates/calendardelegate.h \
    src/delegates/comboobjectitemdelegate.h \
    src/widgets/calendarwidget.h \
    src/widgets/statisticswidget.h \
    src/widgets/qxtstars/qxtglobal.h \
    src/widgets/qxtstars/qxtpimpl.h \
    src/widgets/qxtstars/qxtstars.h \
    src/utility/statisticsservice.h \
    src/utility/utility.h \
    src/models/baseobjecttablemodel.h \
    src/models/comboobjecttablemodel.h \
    src/models/eventtypetablemodel.h \
    src/models/shoemodeltablemodel.h \
    src/models/shoetablemodel.h \
    src/models/intervaltablemodel.h \
    src/views/eventtypeview.h \
    src/views/shoeview.h \
    src/views/shoemakerview.h \
    src/views/shoemodelview.h \
    src/views/weatherview.h \
    src/views/intervalview.h \
    src/views/intervaltypeview.h \
    src/views/votepopupview.h \
    src/views/runnerinfopopupview.h \
    src/views/weatherinfopopupview.h \
    src/views/mainview.h
FORMS += src/views/ui/eventtypeview.ui \
    src/views/ui/shoeview.ui \
    src/views/ui/shoemakerview.ui \
    src/views/ui/shoemodelview.ui \
    src/views/ui/weatherview.ui \
    src/views/ui/intervalview.ui \
    src/views/ui/intervaltypeview.ui \
    src/views/ui/votepopupview.ui \
    src/views/ui/runnerinfopopupview.ui \
    src/views/ui/weatherinfopopupview.ui \
    src/views/ui/mainview.ui
SOURCES += src/objects/baseobject.cpp \
    src/objects/comboobject.cpp \
    src/objects/event.cpp \
    src/objects/eventtype.cpp \
    src/objects/shoe.cpp \
    src/objects/shoemaker.cpp \
    src/objects/shoemodel.cpp \
    src/objects/weather.cpp \
    src/objects/interval.cpp \
    src/objects/intervaltype.cpp \
    src/services/memento.cpp \
    src/services/objectfactory.cpp \
    src/services/objectmap.cpp \
    src/services/objectrepository.cpp \
    src/services/mappers/baseobjectmapper.cpp \
    src/services/mappers/comboobjectmapper.cpp \
    src/services/mappers/eventmapper.cpp \
    src/services/mappers/eventtypemapper.cpp \
    src/services/mappers/shoemapper.cpp \
    src/services/mappers/shoemodelmapper.cpp \
    src/services/mappers/intervalmapper.cpp \
    src/delegates/booleanimageitemdelegate.cpp \
    src/delegates/calendardelegate.cpp \
    src/delegates/comboobjectitemdelegate.cpp \
    src/widgets/calendarwidget.cpp \
    src/widgets/statisticswidget.cpp \
    src/widgets/qxtstars/qxtstars.cpp \
    src/utility/statisticsservice.cpp \
    src/utility/utility.cpp \
    src/models/baseobjecttablemodel.cpp \
    src/models/comboobjecttablemodel.cpp \
    src/models/eventtypetablemodel.cpp \
    src/models/shoemodeltablemodel.cpp \
    src/models/shoetablemodel.cpp \
    src/models/intervaltablemodel.cpp \
    src/views/eventtypeview.cpp \
    src/views/shoeview.cpp \
    src/views/shoemakerview.cpp \
    src/views/shoemodelview.cpp \
    src/views/weatherview.cpp \
    src/views/intervalview.cpp \
    src/views/intervaltypeview.cpp \
    src/views/votepopupview.cpp \
    src/views/runnerinfopopupview.cpp \
    src/views/weatherinfopopupview.cpp \
    src/views/mainview.cpp \
    src/main.cpp
RESOURCES += resources/application.qrc \
    application.qrc
RC_FILE = resources/application.rc
TRANSLATIONS = resources/translations/running_it_IT.ts \
	resources/translations/running_en_US.ts
CODECFORTR = ISO-8859-15
