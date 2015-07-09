#ifndef DLGVALIDATEREGISTRATION_H
#define DLGVALIDATEREGISTRATION_H

#include <QDialog>

namespace Ui {
class DlgValidateRegistration;
}

class DlgValidateRegistration : public QDialog
{
    Q_OBJECT

public:
    explicit DlgValidateRegistration(QWidget *parent = 0);
    ~DlgValidateRegistration();

private:
    Ui::DlgValidateRegistration *ui;
};

#endif // DLGVALIDATEREGISTRATION_H
