QT += core \
network
QT -= gui
TARGET = QCloud
TEMPLATE = lib
CONFIG -= app_bundle
CONFIG += staticlib
LIBS += qcloud.lib


CONFIG(debug, debug|release) {
    DESTDIR =./debug/
} else {
    DESTDIR =./release/
}


SOURCES += \
           qclouditem.cpp \
           qcloudfile.cpp \
           qamazonconnection.cpp \
           qazureconnection.cpp \
           hmacsha.cpp \
           qclouddir.cpp \
           qcloudresponse.cpp \
           qcloudconnection.cpp \
           qcloudlistresponse.cpp \
           qcloudfileresponse.cpp
HEADERS += qclouditem.h \
           qcloudfile.h \
           qcloudconnection.h \
           qamazonconnection.h \
           qazureconnection.h \
           hmacsha.h \
           qclouddir.h \
           qcloudresponse.h \
           qcloudlistresponse.h \
           qcloudfileresponse.h
