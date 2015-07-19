#ifndef DLGREGISTRATION_H
#define DLGREGISTRATION_H

#include <QDialog>

namespace Ui {
class DlgRegistration;
}

class DlgRegistration : public QDialog
{
    Q_OBJECT

public:
    explicit DlgRegistration(bool demoMode, QWidget *parent = 0);
    ~DlgRegistration();

    QString dni() const;
    QString nombre() const;

private slots:

private:
    Ui::DlgRegistration *ui;
    bool _demoMode;
};

#endif // DLGREGISTRATION_H
