#include "dlgvalidateregistration.h"
#include "ui_dlgvalidateregistration.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>

DlgValidateRegistration::DlgValidateRegistration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgValidateRegistration)
{
    ui->setupUi(this);

    QString url_str = "http://www.hbobroker.com.ar/smartcard"
                      "/registration_requests";

    HttpRequestInput input(url_str, "GET");

    HttpRequestWorker *worker = new HttpRequestWorker(this);
    connect(worker, &HttpRequestWorker::on_execution_finished, this, &DlgValidateRegistration::on_dataReceived);
    worker->execute(&input);
}

DlgValidateRegistration::~DlgValidateRegistration()
{
    delete ui;
}

void DlgValidateRegistration::on_dataReceived(HttpRequestWorker *worker)
{
    if (worker->error_type == QNetworkReply::NoError)
    {
        // communication was successful
        QByteArray response = worker->response;

        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        if (jsonDoc.isArray())
        {
            ui->tableWidget->setRowCount(0);
            int currentRow = ui->tableWidget->rowCount();
            for (int index = 0; index < jsonDoc.array().size(); ++index)
            {
                QJsonObject jsonObj = jsonDoc.array()[index].toObject();
                if (jsonObj["fecha_registracion"].isNull())
                {
                    ui->tableWidget->insertRow(currentRow);
                    QTableWidgetItem *item = new QTableWidgetItem(jsonObj["dni"].toString());
                    qDebug() << jsonObj["id"].toString();
                    item->setData(Qt::UserRole, jsonObj["id"].toString());
                    item->setCheckState(Qt::Unchecked);
                    ui->tableWidget->setItem(currentRow, 0, item);

                    item = new QTableWidgetItem(jsonObj["celular"].toString());
                    ui->tableWidget->setItem(currentRow, 1, item);

                    item = new QTableWidgetItem(jsonObj["nombre"].toString());
                    ui->tableWidget->setItem(currentRow, 2, item);

                    item = new QTableWidgetItem(jsonObj["fecha_solicitud"].toString());
                    ui->tableWidget->setItem(currentRow, 3, item);

                    qDebug() << ui->tableWidget->item(currentRow, 0)->data(Qt::UserRole);
                    currentRow ++;
                }
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

void DlgValidateRegistration::on_buttonBox_accepted()
{
    QString urlTempl = "http://www.hbobroker.com.ar/smartcard/approve/%1";

    for (int r = 0; r < ui->tableWidget->rowCount(); ++r)
    {
        QTableWidgetItem *item = ui->tableWidget->item(r, 0);
        if (item->checkState() == Qt::Checked)
        {
            QString url = urlTempl.arg(item->data(Qt::UserRole).toString());

            HttpRequestInput input(url, "GET");
            HttpRequestWorker *w = new HttpRequestWorker(this);
            connect(w, &HttpRequestWorker::on_execution_finished, this, &DlgValidateRegistration::on_updateFinished);
            w->execute(&input);
        }
    }
}


void DlgValidateRegistration::on_updateFinished(HttpRequestWorker *worker)
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
