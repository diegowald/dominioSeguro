#ifndef SCREENTOOLS_H
#define SCREENTOOLS_H

#include <QObject>

class ScreenTools : public QObject
{
    Q_OBJECT
public:
    explicit ScreenTools(QObject *parent = 0);

    int mm2pix(qreal mm);
    int inch2pix(qreal inch);
private:

    qreal _dpi;
};

#endif // SCREENTOOLS_H
