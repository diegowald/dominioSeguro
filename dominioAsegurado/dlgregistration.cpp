#include "dlgregistration.h"
#include "ui_dlgregistration.h"

DlgRegistration::DlgRegistration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgRegistration)
{
    ui->setupUi(this);
}

DlgRegistration::~DlgRegistration()
{
    delete ui;
}
