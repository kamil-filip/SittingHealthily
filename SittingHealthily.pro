#-------------------------------------------------
#
# Project created by QtCreator 2016-08-03T16:27:32
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SittingHealthily
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp \
    qcustomplot.cpp \
    chartthread.cpp \
    mainwidget.cpp \
    geometryengine.cpp \
    streamdecoder.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h \
    qcustomplot.h \
    chartthread.h \
    mainwidget.h \
    geometryengine.h \
    streamdecoder.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    images/resources.qrc \
    textures.qrc \
    shaders.qrc

DISTFILES += \
    vshader.glsl \
    fshader.glsl
