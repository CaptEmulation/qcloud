QT       -= gui
QT += network
TARGET = qcloudtransfer
CONFIG   += console
CONFIG   -= app_bundle

INCLUDEPATH += ../../qcloud

CONFIG(debug, debug|release){
    LIBS += -L../../qcloud/debug -lQCloud
} else {
    LIBS += -L../../qcloud/release -lQCloud
}

TEMPLATE = app


SOURCES += main.cpp
