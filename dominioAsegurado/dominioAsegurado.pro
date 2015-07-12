#-------------------------------------------------
#
# Project created by QtCreator 2015-07-09T08:46:59
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

android: QT += androidextras  #included only in Android builds

TARGET = HBO_Broker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dlgnotimplemented.cpp \
    httprequestworker.cpp

HEADERS  += mainwindow.h \
    dlgnotimplemented.h \
    httprequestworker.h

FORMS    += mainwindow.ui \
    dlgnotimplemented.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    iconresources.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

