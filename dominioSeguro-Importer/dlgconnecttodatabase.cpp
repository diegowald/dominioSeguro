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

QString DlgConnectToDatabase::server() const
{
    return ui->txtServer->text();
}

QString DlgConnectToDatabase::database() const
{
    return ui->txtDatabase->text();
}

QString DlgConnectToDatabase::user() const
{
    return ui->txtUser->text();
}

QString DlgConnectToDatabase::password() const
{
    return ui->txtPassword->text();
}
