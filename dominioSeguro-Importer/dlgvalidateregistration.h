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

private slots:
    void on_dataReceived(HttpRequestWorker *worker);
    void on_updateFinished(HttpRequestWorker *worker);
    void on_buttonBox_accepted();


private:
    Ui::DlgValidateRegistration *ui;
};

#endif // DLGVALIDATEREGISTRATION_H
