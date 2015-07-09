#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlgconnecttodatabase.h"
#include "dlgimportcsv.h"
#include "dlgvalidateregistration.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConnect_to_Server_triggered()
{
    DlgConnectToDatabase dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
    }
}

void MainWindow::on_actionUpload_new_data_triggered()
{
    DlgImportCSV dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
    }
}

void MainWindow::on_actionValidate_Users_triggered()
{
    DlgValidateRegistration dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
    }
}
