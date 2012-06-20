#-------------------------------------------------
#
# Project created by QtCreator 2012-05-30T09:55:40
#
#-------------------------------------------------

QT       += core gui network xml

TARGET = QCloudExample
TEMPLATE = app


SOURCES += main.cpp\
        cloudexample.cpp \
    settingsmenu.cpp \
    qcloudhelper.cpp \
    xmlparser.cpp

HEADERS  += cloudexample.h \
    settingsmenu.h \
    qcloudhelper.h \
    xmlparser.h

FORMS    += cloudexample.ui

RESOURCES += \
    qcloudexample.qrc
