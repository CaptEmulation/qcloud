QT       += network xml testlib

TARGET = tst_qcloudtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
PRE_TARGETDEPS += ../qcloud/release/libQCloud.a
INCLUDEPATH += ../qcloud/
LIBS += -L../qcloud/release -lQCloud

SOURCES += tst_qcloudtest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
