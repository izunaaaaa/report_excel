#-------------------------------------------------
#
# Project created by QtCreator 2022-06-30T06:59:49
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia multimediawidgets
TMAKE_CXXFLAGS += -fexceptions

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImgViewer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += CUDNN OPENCV

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        error_select.cpp \
        main.cpp \
        dialog.cpp \
        maindlg.cpp

HEADERS += \
        dialog.h \
        error_select.h \
        maindlg.h

FORMS += \
        dialog.ui \
        error_select.ui \
        maindlg.ui

QXLSX_PARENTPATH=../Qxlsx/
QXLSX_HEADERPATH=../Qxlsx/header/
QXLSX_SOURCEPATH=../Qxlsx/source/
include(../Qxlsx/QXlsx.pri)

#CONFIG += console

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
