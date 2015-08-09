#ifndef DLGVALIDATEREGISTRATION_H
#define DLGVALIDATEREGISTRATION_H

#include <QDialog>
#include "httprequestworker.h"

namespace Ui {
class DlgValidateRegistration;
}

class DlgValidateRegistration : public QDialog
{
    Q_OBJECT

public:
    explicit DlgValidateRegistration(QWidget *parent = 0);
    ~DlgValidateRegistration();
    QStringList getIdsToValidate();

private slots:
    void on_dataReceived(HttpRequestWorker *worker);


private:
    Ui::DlgValidateRegistration *ui;
};

#endif // DLGVALIDATEREGISTRATION_H
