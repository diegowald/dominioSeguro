#-------------------------------------------------
#
# Project created by QtCreator 2015-07-09T08:46:59
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

android: QT += androidextras  #included only in Android builds

TARGET = SmartCard
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dlgnotimplemented.cpp \
    httprequestworker.cpp \
    dlgregistration.cpp \
    screentools.cpp

HEADERS  += mainwindow.h \
    dlgnotimplemented.h \
    httprequestworker.h \
    dlgregistration.h \
    screentools.h

FORMS    += mainwindow.ui \
    dlgnotimplemented.ui \
    dlgregistration.ui

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
    android/gradlew.bat \
    ../imagenes/asociart.gif \
    ../imagenes/el_surco.gif \
    ../imagenes/Prudencia.gif \
    ../imagenes/RSA.gif \
    ../imagenes/smg-seguros.gif \
    ../imagenes/Berkley.jpg \
    ../imagenes/federacion_patronal.jpg \
    ../imagenes/galeno_art.jpg \
    ../imagenes/integrity.jpg \
    ../imagenes/mercantil_andina.jpg \
    ../imagenes/parana_seguros.jpg \
    ../imagenes/Prevencion.jpg \
    ../imagenes/provart.jpg \
    ../imagenes/Provincia.jpg \
    ../imagenes/qbe.jpeg \
    ../imagenes/Sancor-Seguros.jpg \
    ../imagenes/seguros_ravadavia.jpg \
    ../imagenes/SMGART.jpg

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

