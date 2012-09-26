#-------------------------------------------------
#
# Project created by QtCreator 2012-09-03T09:41:53
#
#-------------------------------------------------

QT       += core gui network

TARGET = QCloudExplorer
TEMPLATE = app

PRE_TARGETDEPS += ../QCloud-build-desktop-Qt_4_8_3__qt__Release/release/libQCloud.a
SOURCES += main.cpp\
        mainwindow.cpp \
    qsettingsdialog.cpp
INCLUDEPATH += ../QCloud-build-desktop-Qt_4_8_3__qt__Release/release
LIBS += -L../QCloud-build-desktop-Qt_4_8_3__qt__Release/release -lQCloud
HEADERS  += mainwindow.h \
    qsettingsdialog.h

FORMS    += mainwindow.ui
