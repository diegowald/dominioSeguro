#include "dlgvalidateregistration.h"
#include "ui_dlgvalidateregistration.h"

DlgValidateRegistration::DlgValidateRegistration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgValidateRegistration)
{
    ui->setupUi(this);
}

DlgValidateRegistration::~DlgValidateRegistration()
{
    delete ui;
}
