#-------------------------------------------------
#
# Project created by QtCreator 2012-02-28T09:41:41
#
#-------------------------------------------------

QT       += core gui network xml

TARGET = RestExample
TEMPLATE = app


SOURCES += main.cpp\
        window.cpp \
    resthandler.cpp \
    dialog.cpp

HEADERS  += window.h \
    resthandler.h \
    dialog.h

FORMS    += window.ui \
    dialog.ui
