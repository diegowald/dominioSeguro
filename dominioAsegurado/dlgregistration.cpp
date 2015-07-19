#include "dlgregistration.h"
#include "ui_dlgregistration.h"

DlgRegistration::DlgRegistration(bool demoMode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgRegistration)
{
    ui->setupUi(this);
    _demoMode = demoMode;
    setWindowState(Qt::WindowMaximized);
}

DlgRegistration::~DlgRegistration()
{
    delete ui;
}

QString DlgRegistration::dni() const
{
    return ui->txtDocumento->text();
}

QString DlgRegistration::nombre() const
{
    return ui->txtNombre->text();
}
