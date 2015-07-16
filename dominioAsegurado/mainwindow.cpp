#include "mainwindow.h"
#include "ui_mainwindow.h"

#if defined(Q_OS_ANDROID)
#include <QtAndroid>
#include <QAndroidJniObject>
#endif

#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "dlgnotimplemented.h"

#include <QMessageBox>
#include <QStandardPaths>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lblAnio->clear();
    ui->lblAsegurado->clear();
    ui->lblChasis->clear();
    ui->lblCobertura->clear();
    ui->lblDominio->clear();
    ui->lblMedioPago->clear();
    ui->lblModelo->clear();
    ui->lblMotor->clear();
    ui->lblPoliza->clear();
    ui->lblProductor->clear();
    ui->lblVigenciaDesde->clear();
    ui->lblVigenciaHasta->clear();
    ui->comboBox->clear();
    dominiosAsegurados.clear();
    _fileDataLocation = QString("%1/%2")
            .arg(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation))
            .arg("data.json");
    if (!_fileDataLocation.isEmpty())
    {
        QFile jsonFile(_fileDataLocation);
        jsonFile.open(QFile::ReadOnly);
        QJsonDocument jsonDoc = QJsonDocument().fromJson(jsonFile.readAll());
        loadJson(jsonDoc);
    }
    else
    {
        _fileDataLocation = QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, "data", QStandardPaths::LocateOption::LocateDirectory);
    }

    _settingsLocation = QString("%1/%2")
            .arg(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation))
            .arg("settings.json");
    if (!_fileDataLocation.isEmpty())
    {
        QFile jsonFile(_settingsLocation);
        jsonFile.open(QFile::ReadOnly);
        QJsonDocument jsonDoc = QJsonDocument().fromJson(jsonFile.readAll());
        loadJsonSettings(jsonDoc);
    }
    else
    {
        _settingsLocation = QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, "data", QStandardPaths::LocateOption::LocateDirectory);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QJsonObject jsonObj = dominiosAsegurados[arg1];
    ui->lblAnio->setText(jsonObj["anio"].toString());
    ui->lblAsegurado->setText(jsonObj["asegurado"].toString());
    ui->lblDominio->setText(jsonObj["dominio"].toString());
    ui->lblCobertura->setText(jsonObj["cobertura"].toString());
    ui->lblPoliza->setText(jsonObj["poliza"].toString());
    ui->lblVigenciaDesde->setText(jsonObj["vigencia_desde"].toString());
    ui->lblVigenciaHasta->setText(jsonObj["vigencia_hasta"].toString());
    ui->lblModelo->setText(jsonObj["modelo"].toString());
    ui->lblChasis->setText(jsonObj["chasis"].toString());
    ui->lblMotor->setText(jsonObj["motor"].toString());
    ui->lblMedioPago->setText(jsonObj["medioPago"].toString());
    ui->lblProductor->setText(jsonObj["Productor"].toString());
}

void MainWindow::handle_result(HttpRequestWorker *worker)
{
    if (worker->error_type == QNetworkReply::NoError)
    {
        // communication was successful
        QByteArray response = worker->response;

        ui->comboBox->clear();
        dominiosAsegurados.clear();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        loadJson(jsonDoc);
        QFile jsonFile(_fileDataLocation);
        jsonFile.open(QFile::WriteOnly);
        jsonFile.write(jsonDoc.toJson());
    }
    else
    {
        // an error occurred
        QString msg = "Error: " + worker->error_str;
        QMessageBox::information(this, "", msg);
    }
}

void MainWindow::loadJson(QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray())
    {
        for (int index = 0; index < jsonDoc.array().size(); ++index)
        {
            QJsonObject jsonObj = jsonDoc.array()[index].toObject();
            dominiosAsegurados[jsonObj["dominio"].toString()] = jsonObj;
            ui->comboBox->addItem(jsonObj["dominio"].toString(), jsonObj);
        }
    }
}

void MainWindow::loadJsonSettings(QJsonDocument &jsonDoc)
{
    if (jsonDoc.isObject())
    {
        QJsonObject jsonObj = jsonDoc.object();
        _dniAsociado = jsonObj["dni"].toString();
    }
}


void MainWindow::on_btnUpdate_clicked()
{
    if (_dniAsociado.length() == 0)
    {
        registrar();
    }
    else
    {
        QString url_str = "http://192.168.0.103/slim/datos/" + _dniAsociado;

        HttpRequestInput input(url_str, "GET");

        HttpRequestWorker *worker = new HttpRequestWorker(this);
        connect(worker, SIGNAL(on_execution_finished(HttpRequestWorker*)), this, SLOT(handle_result(HttpRequestWorker*)));
        worker->execute(&input);
    }
}

void MainWindow::on_btnCallForCrane_clicked()
{
    QString number = "2914139389";
    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
    if (activity.isValid())
    {
        // real Java code to C++ code
        // Intent callIntent = new callIntent(Intent.ACTION_CALL);
        QAndroidJniObject callConstant = QAndroidJniObject::getStaticObjectField<jstring>("android/content/Intent", "ACTION_CALL");
        QAndroidJniObject callIntent("android/content/Intent",  "(Ljava/lang/String;)V", callConstant.object());
        // callIntent.setPackage("com.android.phone"); (<= 4.4w)  intent.setPackage("com.android.server.telecom");  (>= 5)
        QAndroidJniObject package;
        if(QtAndroid::androidSdkVersion() >= 21)
            package = QAndroidJniObject::fromString("com.android.server.telecom");
        else
            package = QAndroidJniObject::fromString("com.android.phone");
        callIntent.callObjectMethod("setPackage", "(Ljava/lang/String;)Landroid/content/Intent;", package.object<jstring>());
        // callIntent.setData(Uri.parse("tel:" + number));
        QAndroidJniObject jNumber = QAndroidJniObject::fromString(QString("tel:%1").arg(number));
        QAndroidJniObject uri = QAndroidJniObject::callStaticObjectMethod("android/net/Uri","parse","(Ljava/lang/String;)Landroid/net/Uri;", jNumber.object());
        callIntent.callObjectMethod("setData", "(Landroid/net/Uri;)Landroid/content/Intent;", uri.object<jobject>());
        // callIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        jint flag = QAndroidJniObject::getStaticField<jint>("android/content/Intent", "FLAG_ACTIVITY_NEW_TASK");
        callIntent.callObjectMethod("setFlags", "(I)Landroid/content/Intent;", flag);
        //startActivity(callIntent);
        activity.callMethod<void>("startActivity","(Landroid/content/Intent;)V", callIntent.object<jobject>());
    }
    else
        qDebug() << "Something wrong with Qt activity...";
}

void MainWindow::on_btnCallForCrash_clicked()
{
    DlgNotImplemented dlg(this);
    dlg.exec();
}

void MainWindow::on_btnCallForInformation_clicked()
{
    DlgNotImplemented dlg(this);
    dlg.exec();
}
