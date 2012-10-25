#-------------------------------------------------
#
# Project created by QtCreator 2012-09-03T09:41:53
#
#-------------------------------------------------

QT       += core gui network widgets

TARGET = QCloudExplorer
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qsettingsdialog.cpp

HEADERS += \
    mainwindow.h \
    qsettingsdialog.h

INCLUDEPATH += ../../qcloud

CONFIG(debug, debug|release) {
    LIBS += -L../../qcloud/debug -lQCloud
} else {
    LIBS += -L../../qcloud/release -lQCloud
}

FORMS    += mainwindow.ui
