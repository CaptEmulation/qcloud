# -------------------------------------------------
# Project created by QtCreator 2012-07-19T10:01:20
# -------------------------------------------------
QT += core \
    network
QT -= gui
TARGET = QCloud

TEMPLATE = lib
CONFIG -= app_bundle
CONFIG += staticlib
LIBS += qcloud.lib
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
