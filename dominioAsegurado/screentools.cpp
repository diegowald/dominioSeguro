#include "screentools.h"
#include <QApplication>
#include <QScreen>

#define MM2INCH 0.0393700787401575
#define INCH2MM 25.4

ScreenTools::ScreenTools(QObject *parent) : QObject(parent)
{
    _dpi = QApplication::primaryScreen()->physicalDotsPerInch();
}

int ScreenTools::mm2pix(qreal mm)
{
    qreal inch = mm * MM2INCH;

    return inch2pix(inch);
}

int ScreenTools::inch2pix(qreal inch)
{
    qreal px = inch * _dpi;

    return (int)(px + 0.5);
}

