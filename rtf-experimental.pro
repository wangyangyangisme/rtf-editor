#-------------------------------------------------
#
# Project created by QtCreator 2016-07-04T15:15:19
#
#-------------------------------------------------

QT      += core gui widgets
SOURCES += main.cpp maineditor.cpp \
    setupmaineditor.cpp \
    Tables/insertrowdialog.cpp \
    Tables/tabledialog.cpp \
    Tables/tablewindow.cpp \
    Tables/tableactions.cpp \
    textactions.cpp
HEADERS += maineditor.h \
    Tables/insertrowdialog.h \
    Tables/tabledialog.h \
    Tables/tablewindow.h
TARGET   = rtf-experimental
TEMPLATE = app

FORMS += \
    Tables/insertrowdialog.ui \
    Tables/tabledialog.ui

RESOURCES += \
    resources.qrc
