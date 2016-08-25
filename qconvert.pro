#-------------------------------------------------
#
# Project created by QtCreator 2016-08-24T11:11:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qconvert
TEMPLATE = app


SOURCES += main.cpp\
    QConvert.cpp \
    QVideoFormat.cpp

HEADERS  += QConvert.hpp \
    QVideoFormat.hpp \
    QHelper.hpp

FORMS    += QConvert.ui

OTHER_FILES += \
    .gitignore \
    README.md \
    license.txt
