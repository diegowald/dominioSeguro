#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlgconnecttodatabase.h"
#include "dlgimportcsv.h"
#include "dlgvalidateregistration.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "httprequestworker.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include "csvreader.h"
#include "recordupdater.h"
#include <QDebug>

// http://www.creativepulse.gr/en/blog/2014/restful-api-requests-using-qt-cpp-for-linux-mac-osx-ms-windows

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
        QStringList validations = dlg.getIdsToValidate();

        QString url = "http://www.hbobroker.com.ar/smartcard/approve";

        foreach (QString id, validations)
        {
            HttpRequestInput input(url, "POST");

            input.add_var("id", id);

            HttpRequestWorker *worker = new HttpRequestWorker(this);
            connect(worker, SIGNAL(on_execution_finished(HttpRequestWorker*)), this, SLOT(on_updateFinished(HttpRequestWorker*)));
            worker->execute(&input);
        }

        on_actionRefresh_triggered();
    }
}

void MainWindow::on_updateFinished(HttpRequestWorker *worker)
{
    if (worker->error_type == QNetworkReply::NoError)
    {
        // communication was successful
        //QByteArray response = worker->response;
    }
    else
    {
        // an error occurred
        QString msg = "Error: " + worker->error_str;
        QMessageBox::information(this, "", msg);
    }

    worker->deleteLater();
}

void MainWindow::uploadData()
{
    QString url = "http://www.hbobroker.com.ar/smartcard/uploadcsv";

    HttpRequestInput input(url, "POST");

    input.add_file("upload", _filename, NULL, "text/csv");

    HttpRequestWorker *worker = new HttpRequestWorker();
    connect(worker, &HttpRequestWorker::on_execution_finished, this, &MainWindow::on_uploadCSVfinished);
    worker->execute(&input);
}

void MainWindow::on_uploadCSVfinished(HttpRequestWorker *worker)
{
    if (worker->error_type == QNetworkReply::NoError)
    {
        // communication was successful
        QByteArray response = worker->response;
        qDebug() << QString(response);
    }
    else
    {
        // an error occurred
        QString msg = "Error: " + worker->error_str;
        QMessageBox::information(this, "", msg);
    }

    worker->deleteLater();
}

void MainWindow::uploadData2()
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

void MainWindow::uploadData3()
{
    _updater = new RecordUpdater(_filename, _columnSeparator,
                  _numLinesToIgnore, _stringDelimiter);

    connect(_updater, &RecordUpdater::processFinished, this, &MainWindow::on_updateRecordsFinished);
    _updater->run();
}

void MainWindow::on_updateRecordsFinished()
{
    ui->statusBar->showMessage("update Finalizado", 3);
}

void MainWindow::on_actionRefresh_triggered()
{
    QString url_str = "http://www.hbobroker.com.ar/smartcard"
                      "/stats";

    HttpRequestInput input(url_str, "GET");

    HttpRequestWorker *worker = new HttpRequestWorker(this);
    connect(worker, &HttpRequestWorker::on_execution_finished, this, &MainWindow::on_statsReceived);
    worker->execute(&input);
}

void MainWindow::on_statsReceived(HttpRequestWorker *worker)
{
    if (worker->error_type == QNetworkReply::NoError)
    {
        // communication was successful
        QByteArray response = worker->response;

        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        if (jsonDoc.isArray())
        {
            for (int index = 0; index < jsonDoc.array().size(); ++index)
            {
                QJsonObject jsonObj = jsonDoc.array()[index].toObject();
                //dominiosAsegurados[jsonObj["dominio"].toString()] = jsonObj;
                //ui->comboBox->addItem(jsonObj["dominio"].toString(), jsonObj);
                ui->lblCountSeguros->setText(jsonObj["countDatos"].toString());
                ui->lblRegistraciones->setText(jsonObj["totalRegistrados"].toString());
                ui->lblRequests->setText(jsonObj["regSolicitados"].toString());
//                "[{"id":"1","countDatos":"1","totalRegistrados":"5","regsSolicitados":"5"}]"
            }
        }
    }
    else
    {
        // an error occurred
        QString msg = "Error: " + worker->error_str;
        QMessageBox::information(this, "", msg);
    }
    worker->deleteLater();
}
