# Copyright (C) 2018 Yuxiang Sun
# Email: yxsun@link.cuhk.edu.hk
# MIT License

QT       += core gui
QT       += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport


TARGET = wifi_localization
TEMPLATE = app


SOURCES	+= main.cpp\
        mainwindow.cpp \
		qcustomplot.cpp \
		localization.cpp

HEADERS	+= mainwindow.h \
		qcustomplot.h \
		localization.h

FORMS	+= mainwindow.ui

OTHER_FILES +=

RESOURCES += \
    wifi_localization.qrc
