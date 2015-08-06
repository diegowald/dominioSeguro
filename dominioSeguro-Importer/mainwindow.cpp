#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlgconnecttodatabase.h"
#include "dlgimportcsv.h"
#include "dlgvalidateregistration.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "httprequestworker.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), _server(""), _database(""),
    _user(""), _password("")
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
        _server = dlg.server();
        _database = dlg.database();
        _user = dlg.user();
        _password = dlg.password();
    }
}

void MainWindow::on_actionUpload_new_data_triggered()
{
    DlgImportCSV dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        _filename = dlg.filename();
        _columnSeparator = dlg.columnSeparator();
        _stringDelimiter = dlg.stringDelimiter();
        _recordSeparator = dlg.recordSeparator();
        _numLinesToIgnore = dlg.numLinesToIgnore();
        uploadData();
    }
}

void MainWindow::on_actionValidate_Users_triggered()
{
    DlgValidateRegistration dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
    }
}

void MainWindow::uploadData()
{
    QString sqlQuery = " LOAD DATA LOCAL INFILE '%1' INTO TABLE datos "
                       " FIELDS TERMINATED BY '%2' OPTIONALY ENCLOSED BY '%3' "
                       " LINES TERMINATED BY '%4' "
                       " IGNORE %5 LINES; ";

    QString sql = sqlQuery.arg(_filename).arg(_columnSeparator).arg(_stringDelimiter)
            .arg(_recordSeparator).arg(_numLinesToIgnore);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(_server);
    db.setDatabaseName(_database);
    db.setUserName(_user);
    db.setPassword(_password);

    qDebug() << QSqlDatabase::drivers().join(", ");
    if (db.open())
    {
        QSqlQuery qry(sql);
        if (qry.exec())
        {
            // todo bien
        }
    }
    db.close();
}

void MainWindow::on_actionRefresh_triggered()
{
    QString url_str = "http://www.hbobroker.com.ar/smartcard"
                      "/register";

    HttpRequestInput input(url_str, "POST");

    input.add_var("dni", dlg.dni());
    input.add_var("celular", "");
    input.add_var("nombre", dlg.nombre());
    input.add_var("fechaSolicitud", QDate::currentDate().toString("yyyy-MM-dd"));

    HttpRequestWorker *worker = new HttpRequestWorker(this);
    connect(worker, SIGNAL(on_execution_finished(HttpRequestWorker*)), this, SLOT(handle_resultRegistration(HttpRequestWorker*)));
    worker->execute(&input);
cp
}
