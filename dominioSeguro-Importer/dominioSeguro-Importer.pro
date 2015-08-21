#-------------------------------------------------
#
# Project created by QtCreator 2015-07-08T09:26:29
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dominioSeguro-Importer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dlgimportcsv.cpp \
    dlgvalidateregistration.cpp \
    httprequestworker.cpp \
    csvreader.cpp \
    #recordupdater.cpp \
    csvwriter.cpp

HEADERS  += mainwindow.h \
    dlgimportcsv.h \
    dlgvalidateregistration.h \
    httprequestworker.h \
    csvreader.h \
#    recordupdater.h \
    csvwriter.h

FORMS    += mainwindow.ui \
    dlgimportcsv.ui \
    dlgvalidateregistration.ui
