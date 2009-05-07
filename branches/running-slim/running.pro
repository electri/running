TARGET = Running
unix:TARGET = running
TEMPLATE = app
QT += sql
CONFIG += qt \
    debug_and_release
CONFIG(debug, debug|release) { 
    CONFIG += debug \
        console
    DESTDIR = bin/debug
    OBJECTS_DIR = obj/debug
}
else { 
    DESTDIR = bin/release
    OBJECTS_DIR = obj/release
}
UI_DIR = obj
MOC_DIR = obj
RCC_DIR = obj
INCLUDEPATH += src \
    resources \
    obj
DEPENDPATH += src \
    resources \
    obj
HEADERS += src/views/mainview.h \
    src/widgets/calendarwidget/calendarwidgetinternal.h \
    src/widgets/calendarwidget/calendarwidget.h \
    src/widgets/qxtstars/qxtstars.h \
    src/widgets/qxtstars/qxtpimpl.h \
    src/widgets/qxtstars/qxtglobal.h \
    src/widgets/calendarwidget/calendarwidgetdelegate.h \
    src/utility/database.h \
    src/utility/utility.h \
    src/objects/eventgateway.h \
    src/views/popupviews/weatherinfopopupview.h \
    src/views/popupviews/votepopupview.h \
    src/views/popupviews/runnerinfopopupview.h \
    src/views/tableviews/eventtypeview.h \
    src/delegates/durationstyleditemdelegate.h \
    src/delegates/distancestyleditemdelegate.h \
    src/delegates/booleanimageitemdelegate.h \
    src/views/tableviews/abstracttableview.h \
    src/views/tableviews/weatherview.h \
    src/views/tableviews/intervaltypeview.h \
    src/utility/completerhelper.h \
    src/utility/comboboxhelper.h \
    src/objects/eventfinder.h \
    src/widgets/statisticswidget/statisticswidget.h \
    src/views/settingsview.h \
    src/delegates/comboboxitemdelegate.h \
    src/views/tableviews/shoeview.h \
    src/objects/shoegateway.h \
    src/objects/shoefinder.h \
    src/views/tableviews/shoemakerview.h \
    src/views/tableviews/shoemodelview.h \
    src/settings.h \
    src/widgets/eventwidget/eventwidget.h
FORMS += src/views/mainview.ui \
    src/widgets/calendarwidget/calendarwidget.ui \
    src/views/popupviews/weatherinfopopupview.ui \
    src/views/popupviews/votepopupview.ui \
    src/views/popupviews/runnerinfopopupview.ui \
    src/views/tableviews/eventtypeview.ui \
    src/views/tableviews/weatherview.ui \
    src/views/tableviews/intervaltypeview.ui \
    src/views/settingsview.ui \
    src/views/tableviews/shoeview.ui \
    src/views/tableviews/shoemakerview.ui \
    src/views/tableviews/shoemodelview.ui \
    src/widgets/eventwidget/eventwidget.ui
SOURCES += src/main.cpp \
    src/views/mainview.cpp \
    src/widgets/calendarwidget/calendarwidgetinternal.cpp \
    src/widgets/calendarwidget/calendarwidget.cpp \
    src/widgets/qxtstars/qxtstars.cpp \
    src/widgets/calendarwidget/calendarwidgetdelegate.cpp \
    src/utility/database.cpp \
    src/utility/utility.cpp \
    src/objects/eventgateway.cpp \
    src/views/popupviews/weatherinfopopupview.cpp \
    src/views/popupviews/votepopupview.cpp \
    src/views/popupviews/runnerinfopopupview.cpp \
    src/views/tableviews/eventtypeview.cpp \
    src/delegates/durationstyleditemdelegate.cpp \
    src/delegates/distancestyleditemdelegate.cpp \
    src/delegates/booleanimageitemdelegate.cpp \
    src/views/tableviews/abstracttableview.cpp \
    src/views/tableviews/weatherview.cpp \
    src/views/tableviews/intervaltypeview.cpp \
    src/utility/completerhelper.cpp \
    src/utility/comboboxhelper.cpp \
    src/objects/eventfinder.cpp \
    src/widgets/statisticswidget/statisticswidget.cpp \
    src/views/settingsview.cpp \
    src/delegates/comboboxitemdelegate.cpp \
    src/views/tableviews/shoeview.cpp \
    src/objects/shoegateway.cpp \
    src/objects/shoefinder.cpp \
    src/views/tableviews/shoemakerview.cpp \
    src/views/tableviews/shoemodelview.cpp \
    src/settings.cpp \
    src/widgets/eventwidget/eventwidget.cpp
RESOURCES += resources/application.qrc
win32:RC_FILE = resources/application.rc
macx:ICON = resources/running.icns
