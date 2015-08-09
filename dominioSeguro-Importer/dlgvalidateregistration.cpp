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

QStringList DlgValidateRegistration::getIdsToValidate()
{
    QStringList lst;
    for (int index = 0; index < ui->tableWidget->rowCount(); ++index)
    {
        QTableWidgetItem *item = ui->tableWidget->item(index, 0);
        if (item->checkState() == Qt::Checked)
        {
            lst.append(item->data(Qt::UserRole).toString());
        }
    }
    return lst;
}
