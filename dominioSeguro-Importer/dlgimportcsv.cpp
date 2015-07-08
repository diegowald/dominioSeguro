#include "dlgimportcsv.h"
#include "ui_dlgimportcsv.h"

DlgImportCSV::DlgImportCSV(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgImportCSV)
{
    ui->setupUi(this);
}

DlgImportCSV::~DlgImportCSV()
{
    delete ui;
}
