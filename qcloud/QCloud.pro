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
SOURCES += main.cpp \
    qclouditem.cpp \
    qcloudfile.cpp \
    qcloudtable.cpp \
    qcloudconnection.cpp \
    qamazonconnection.cpp \
    qcloudbucket.cpp \
    qazureconnection.cpp \
    hmacsha.cpp
HEADERS += qclouditem.h \
    qcloudfile.h \
    qcloudtable.h \
    qcloudconnection.h \
    qamazonconnection.h \
    main.h \
    qcloudbucket.h \
    qazureconnection.h \
    hmacsha.h
