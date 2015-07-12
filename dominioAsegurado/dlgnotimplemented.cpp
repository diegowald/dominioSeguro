#include "dlgnotimplemented.h"
#include "ui_dlgnotimplemented.h"

DlgNotImplemented::DlgNotImplemented(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgNotImplemented)
{
    ui->setupUi(this);
}

DlgNotImplemented::~DlgNotImplemented()
{
    delete ui;
}
