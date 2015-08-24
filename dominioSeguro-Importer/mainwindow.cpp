#include "mainwindow.h"
#include "ui_mainwindow.h"
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
#include "csvwriter.h"
//#include "recordupdater.h"

#include <QDebug>

// http://www.creativepulse.gr/en/blog/2014/restful-api-requests-using-qt-cpp-for-linux-mac-osx-ms-windows

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _file.open();
    _file.close();
    on_actionRefresh_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
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
    ui->statusBar->showMessage("Actualizando datos", 3);
    QString url = "http://www.hbobroker.com.ar/smartcard/uploadcsv";

    HttpRequestInput input(url, "POST");

    QString filename = transformCSV();


    input.add_file("upload", filename, NULL, "text/csv");
    input.add_var("columnSeparator", ",");
    input.add_var("stringDelimiter", "\"");
    input.add_var("recordSeparator", "\n");
    input.add_var("numLinesToIgnore", "1");

    HttpRequestWorker *worker = new HttpRequestWorker();
    connect(worker, &HttpRequestWorker::on_execution_finished, this, &MainWindow::on_uploadCSVfinished);
    worker->execute(&input);
}

QString MainWindow::transformCSV()
{
    CSVReader reader(_filename, _columnSeparator, _numLinesToIgnore, _stringDelimiter);
    CSVWriter writer(_file.fileName(), ",", "\"");

    QStringList header;

    reader.load();

    header << "compania" << "dni" << "dominio" << "asegurado"
           << "cobertura" << "poliza" << "vigencia_desde"
           << "vigencia_hasta" << "marca" << "modelo" << "anio"
           << "chasis" << "motor" << "medioPago" << "Productor";

    writer.setHeaders(header);

    for (int i = 0; i < reader.recordCount(); ++i)
    {
        QStringList rec = reader.record(i);
        QStringList record;
        record << rec.at(0)
               << rec.at(8)
               << rec.at(9)
               << rec.at(6)
               << rec.at(22)
               << rec.at(1)
               << rec.at(4)
               << rec.at(5)
               << rec.at(10)
               << rec.at(11)
               << rec.at(12)
               << rec.at(15)
               << rec.at(14)
               << "S/D"
               << rec.at(24);
        writer.addRecord(record);
    }

    return _file.fileName();
}

void MainWindow::on_uploadCSVfinished(HttpRequestWorker *worker)
{
    if (worker->error_type == QNetworkReply::NoError)
    {
        // communication was successful
        QByteArray response = worker->response;
        qDebug() << QString(response);
        ui->statusBar->showMessage("update Finalizado", 3);
    }
    else
    {
        // an error occurred
        QString msg = "Error: " + worker->error_str;
        qDebug() << worker->error_str;
        qDebug() << worker->error_type;

        ui->statusBar->showMessage(msg, 3);
    }

    worker->deleteLater();
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
qDebug() << response;
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
