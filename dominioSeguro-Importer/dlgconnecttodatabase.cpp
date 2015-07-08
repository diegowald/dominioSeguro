#include "dlgconnecttodatabase.h"
#include "ui_dlgconnecttodatabase.h"

DlgConnectToDatabase::DlgConnectToDatabase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgConnectToDatabase)
{
    ui->setupUi(this);
}

DlgConnectToDatabase::~DlgConnectToDatabase()
{
    delete ui;
}
