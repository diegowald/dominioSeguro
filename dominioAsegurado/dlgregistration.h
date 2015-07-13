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
    explicit DlgRegistration(QWidget *parent = 0);
    ~DlgRegistration();

private:
    Ui::DlgRegistration *ui;
};

#endif // DLGREGISTRATION_H
