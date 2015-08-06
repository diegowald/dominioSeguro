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
            for (int index = 0; index < jsonDoc.array().size(); ++index)
            {
                QJsonObject jsonObj = jsonDoc.array()[index].toObject();
                //dominiosAsegurados[jsonObj["dominio"].toString()] = jsonObj;
                //ui->comboBox->addItem(jsonObj["dominio"].toString(), jsonObj);
                qDebug() << jsonObj["dni"].toString();
                qDebug() << jsonObj["celular"].toString();
                qDebug() << jsonObj["nombre"].toString();
                qDebug() << jsonObj["fecha_solicitud"].toString();
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
