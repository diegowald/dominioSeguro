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
#include "dlgregistration.h"

#include <QMessageBox>
#include <QStandardPaths>
#include <QFile>


//#define DEMO

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lblAsegurado->clear();
    ui->lblCompania->clear();
    ui->lblCobertura->clear();
    ui->lblPoliza->clear();
    ui->lblVigenciaDesde->clear();
    ui->lblVigenciaHasta->clear();
    ui->lblMarca->clear();
    ui->lblModelo->clear();
    ui->lblDominio->clear();
    ui->lblAnio->clear();
    ui->lblChasis->clear();
    ui->lblMotor->clear();
    ui->lblMedioPago->clear();
    ui->lblProductor->clear();
    ui->comboBox->clear();
    ui->lblLogo->clear();
    dominiosAsegurados.clear();

    _fileDataLocation = QString("%1/%2")
            .arg(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation))
            .arg("data.json");
    if (!_fileDataLocation.isEmpty())
    {
        QFile jsonFile(_fileDataLocation);
        if (jsonFile.exists())
        {
            jsonFile.open(QFile::ReadOnly);
            QJsonDocument jsonDoc = QJsonDocument().fromJson(jsonFile.readAll());
            loadJson(jsonDoc);
        }
    }
    else
    {
        _fileDataLocation = QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, "data", QStandardPaths::LocateOption::LocateDirectory);
    }

    _settingsLocation = QString("%1/%2")
            .arg(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation))
            .arg("settings.json");
    if (!_settingsLocation.isEmpty())
    {
        QFile jsonFile(_settingsLocation);
        if (jsonFile.exists())
        {
            jsonFile.open(QFile::ReadOnly);
            QJsonDocument jsonDoc = QJsonDocument().fromJson(jsonFile.readAll());
            qDebug() << jsonDoc.toJson();
            loadJsonSettings(jsonDoc);
        }
    }
    else
    {
        _settingsLocation = QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, "data", QStandardPaths::LocateOption::LocateDirectory);
    }
#ifdef DEMO
    ui->lblTitle->setText(ui->lblTitle->text() + "DEMO");
#endif



    QString s = "QLabel { background-color : #f0f0f0; color : black; }";
    ui->lblAsegurado->setStyleSheet(s);
    ui->lblCompania->setStyleSheet(s);
    ui->lblCobertura->setStyleSheet(s);
    ui->lblPoliza->setStyleSheet(s);
    ui->lblVigenciaDesde->setStyleSheet(s);
    ui->lblVigenciaHasta->setStyleSheet(s);
    ui->lblMarca->setStyleSheet(s);
    ui->lblModelo->setStyleSheet(s);
    ui->lblDominio->setStyleSheet(s);
    ui->lblAnio->setStyleSheet(s);
    ui->lblChasis->setStyleSheet(s);
    ui->lblMotor->setStyleSheet(s);
    ui->lblMedioPago->setStyleSheet(s);
    ui->lblProductor->setStyleSheet(s);

    s = "QLabel { background-color : transparent; color : darkred; }";
    ui->lblTitle->setStyleSheet(s);

    s = "QLabel { background-color : white; color : blue; }";
    ui->lbltiuloTarjeta->setStyleSheet(s);
    ui->lblsubtituloTarjeta->setStyleSheet(s);

    QString sf = "QFrame { background-color : white; color : black; }";
    ui->frame->setStyleSheet(sf);

    s = "QToolButton { background-color : yellow; color : black; }";
    ui->btnCrane->setStyleSheet(s);
    ui->btnCrash->setStyleSheet(s);
    ui->btnFeedback->setStyleSheet(s);
    ui->btnInformation->setStyleSheet(s);

    QPalette* palette = new QPalette();
    QLinearGradient linearGradient(QPointF(0, 0), geometry().bottomRight());
    linearGradient.setColorAt(0, Qt::lightGray);
    linearGradient.setColorAt(1, Qt::white);
    palette->setBrush(QPalette::Window,*(new QBrush(linearGradient)));
    setPalette(*palette);


    QSize sz;
    sz.setWidth(_screenTools.mm2pix(8));
    sz.setHeight(_screenTools.mm2pix(8));
    ui->btnCrane->setIconSize(sz);
    ui->btnCrash->setIconSize(sz);
    ui->btnFeedback->setIconSize(sz);
    ui->btnInformation->setIconSize(sz);
    ui->btnGetInformationUpdates->setIconSize(sz);

    ui->lblLogo->setScaledContents(true);
    ui->lblLogo->setMaximumHeight(_screenTools.mm2pix(12));
    ui->lblLogo->setMaximumWidth(_screenTools.mm2pix(12) * 450 / 248);



    // Deshabilitar por ahora
    ui->btnCrane->setEnabled(false);
    ui->btnCrash->setEnabled(false);
    ui->btnFeedback->setEnabled(false);
    ui->btnInformation->setEnabled(false);

    _dlgListadoDNIS = NULL;

    ui->lblSpinner->clear();
    _spinnerMovie = new QMovie(":/img/spinner");
    ui->lblSpinner->setMovie(_spinnerMovie);
    s = "QLabel { background-color : transparent; color : darkred; }";
    ui->lblSpinner->setStyleSheet(s);
    ui->lblSpinner->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QJsonObject jsonObj = dominiosAsegurados[arg1];
    ui->lblCompania->setText(jsonObj["compania"].toString());
    setLogo(jsonObj["compania"].toString());
    ui->lblMarca->setText(jsonObj["marca"].toString());
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
    ui->lblCompania->setText(jsonObj["compania"].toString());
}

void MainWindow::handle_resultUpdate(HttpRequestWorker *worker)
{
    stopSpinner();
    if (worker->error_type == QNetworkReply::NoError)
    {
        // communication was successful
        QByteArray response = worker->response;

        ui->comboBox->clear();
        //dominiosAsegurados.clear();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        loadJson(jsonDoc);

        QFile jsonFile(_fileDataLocation);
        jsonFile.open(QFile::WriteOnly);

        QJsonDocument json2Write;
        QJsonArray array;
        foreach (QJsonObject jsonObj, dominiosAsegurados.values())
        {
            array.append(jsonObj);
        }
        json2Write.setArray(array);

        //jsonFile.write(jsonDoc.toJson());
        jsonFile.write(json2Write.toJson());
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
        }
    }

    ui->comboBox->clear();
    foreach (QJsonObject jsonObj, dominiosAsegurados.values())
    {
        ui->comboBox->addItem(jsonObj["dominio"].toString(), jsonObj);
    }
}

void MainWindow::loadJsonSettings(QJsonDocument &jsonDoc)
{
    if (jsonDoc.isArray())
    {
        for (int i = 0; i < jsonDoc.array().count(); ++i)
        {
            QJsonObject jsonObj = jsonDoc.array().at(i).toObject();
            QString dniAsociado = jsonObj["dni"].toString();
            QString nombre = jsonObj["asegurado"].toString();
            if (dniAsociado.trimmed().length() > 0)
            {
                _dnisAsociado[dniAsociado] = nombre;
            }
        }
    }
}


void MainWindow::on_RequestRegistration(const QString &DNI)
{
    QString url_str = "http://www.hbobroker.com.ar/smartcard"
                      "/register";

    HttpRequestInput input(url_str, "POST");

    input.add_var("dni", DNI);
    input.add_var("celular", "");
    input.add_var("nombre", "");
    input.add_var("fechaSolicitud", QDate::currentDate().toString("yyyy-MM-dd"));

    HttpRequestWorker *worker = new HttpRequestWorker(this);
    connect(worker, SIGNAL(on_execution_finished(HttpRequestWorker*)), this, SLOT(handle_resultRegistration(HttpRequestWorker*)));
    worker->execute(&input);
    startSpinner();
}


void MainWindow::handle_resultRegistration(HttpRequestWorker *worker)
{
    stopSpinner();
    if (worker->error_type == QNetworkReply::NoError)
    {
        // communication was successful
        QByteArray response = worker->response;

        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        loadJsonSettings(jsonDoc);
        QFile jsonFile(_settingsLocation);
        jsonFile.open(QFile::WriteOnly);

        QJsonDocument json2Write;
        QJsonArray array;
        foreach (QString dni, _dnisAsociado.keys())
        {
            QJsonObject jsonObj;
            jsonObj["dni"] = dni;
            jsonObj["asegurado"] = _dnisAsociado[dni];
            array.append(jsonObj);
        }
        json2Write.setArray(array);

        jsonFile.write(json2Write.toJson());
        if (_dlgListadoDNIS != NULL)
        {
            _dlgListadoDNIS->setDNIs(_dnisAsociado);
        }
    }
    else
    {
        // an error occurred
        //QString msg = "Error: " + worker->error_str;
        QString msg = "No se encuentra el DNI ingresado.";
        QMessageBox::information(this, "", msg);
    }
}

void MainWindow::on_removeDNI(const QString &DNI)
{
    _dnisAsociado.remove(DNI);
    QFile jsonFile(_settingsLocation);
    jsonFile.open(QFile::WriteOnly);

    QJsonDocument json2Write;
    QJsonArray array;
    foreach (QString dni, _dnisAsociado.keys())
    {
        QJsonObject jsonObj;
        jsonObj["dni"] = dni;
        jsonObj["asegurado"] = _dnisAsociado[dni];
        array.append(jsonObj);
    }
    json2Write.setArray(array);

    jsonFile.write(json2Write.toJson());

    if (_dlgListadoDNIS != NULL)
    {
        _dlgListadoDNIS->setDNIs(_dnisAsociado);
    }

    QStringList dominiosABorrar;
    foreach (QString dominio, dominiosAsegurados.keys())
    {
        QJsonObject info = dominiosAsegurados[dominio];
        if (info["dni"].toString() == DNI)
        {
            dominiosABorrar.append(dominio);
        }
    }
    foreach (QString dominio, dominiosABorrar)
    {
        dominiosAsegurados.remove(dominio);
    }
    QFile jsonFile2(_fileDataLocation);
    jsonFile2.open(QFile::WriteOnly);

    QJsonDocument json2Write2;
    QJsonArray array2;
    foreach (QJsonObject jsonObj, dominiosAsegurados.values())
    {
        array2.append(jsonObj);
    }
    json2Write2.setArray(array);
    jsonFile2.write(json2Write.toJson());
    loadJson(json2Write2);
}

void MainWindow::on_btnGetInformationUpdates_pressed()
{
    _dlgListadoDNIS = new DialogLIstaDNIS(this);

    connect(_dlgListadoDNIS, &DialogLIstaDNIS::requestRegistration, this, &MainWindow::on_RequestRegistration);
    connect(_dlgListadoDNIS, &DialogLIstaDNIS::removeDNI, this, &MainWindow::on_removeDNI);
    _dlgListadoDNIS->setDNIs(_dnisAsociado);
    if (_dlgListadoDNIS->exec() == QDialog::Accepted)
    {
#ifdef DEMO
        QString json = "[{\"id\":\"1\",\"dni\":\"22943587\",\"dominio\":\"ESK624\",\"asegurado\":\"diego\",\"cobertura\":\"w\",\"poliza\":\"asd\",\"vigencia_desde\":\"1\/1\/2015\",\"vigencia_hasta\":\"31\/12\/2015\",\"modelo\":\"ads\",\"anio\":\"asd\",\"chasis\":\"qe\",\"motor\":\"qwe\",\"medioPago\":\"qwe\",\"Productor\":\"qwe\"},{\"id\":\"2\",\"dni\":\"22943587\",\"dominio\":\"qwe\",\"asegurado\":\"diego\",\"cobertura\":\"654\",\"poliza\":\"89\",\"vigencia_desde\":\"654\",\"vigencia_hasta\":\"89\",\"modelo\":\"684\",\"anio\":\"684\",\"chasis\":\"684\",\"motor\":\"684\",\"medioPago\":\"684\",\"Productor\":\"648\"}]";
        ui->comboBox->clear();
        dominiosAsegurados.clear();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
        loadJson(jsonDoc);
        QFile jsonFile(_fileDataLocation);
        jsonFile.open(QFile::WriteOnly);
        jsonFile.write(jsonDoc.toJson());
#else
        _dnisAsociado = _dlgListadoDNIS->dnis();
        foreach (QString dni, _dnisAsociado.keys())
        {
            QString url_str = "http://www.hbobroker.com.ar/smartcard/datos/" + dni;

            HttpRequestInput input(url_str, "GET");

            HttpRequestWorker *worker = new HttpRequestWorker(this);
            connect(worker, SIGNAL(on_execution_finished(HttpRequestWorker*)), this, SLOT(handle_resultUpdate(HttpRequestWorker*)));
            worker->execute(&input);
            startSpinner();
        }
#endif
    }
    _dlgListadoDNIS->deleteLater();
    _dlgListadoDNIS = NULL;
}

void MainWindow::on_btnCrane_pressed()
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

void MainWindow::on_btnCrash_pressed()
{
    DlgNotImplemented dlg(this);
    dlg.exec();
}


void MainWindow::on_btnFeedback_pressed()
{
    DlgNotImplemented dlg(this);
    dlg.exec();
}

void MainWindow::on_btnInformation_pressed()
{
    DlgNotImplemented dlg(this);
    dlg.exec();
}

void MainWindow::setLogo(const QString &compania)
{
    QString picName = "";
    if (compania == "ROYAL SUNALLIANCE SEG. ARG. SA")
    {
        picName = ":/img/rsa";
    }
    else if (compania == "PARANA SEGUROS")
    {
        picName = ":/img/parana";
    }
    else if (compania == "INTEGRITY SEGUROS")
    {
        picName = ":/img/integrity";
    }
    else if (compania == "FEDERACION PATRONAL S.A.")
    {
        picName = ":/img/federacionpatronal";
    }
    else if (compania == "SANCOR COOPERATIVA DE SEG LTDA")
    {
        picName = ":/img/sancor";
    }
    else if (compania == "PROVINCIA SEGUROS S.A.")
    {
        picName = ":/img/provincia";
    }
    else if (compania == "QBE SEGUROS LA BUENOS AIRES")
    {
        picName = ":/img/qbe";
    }
    else if (compania == "BERKLEY INTERNATIONAL SEG S.A")
    {
        picName = ":/img/berkley";
    }
    else if (compania == "SEGUROS RIVADAVIA")
    {
        picName = ":/img/rivadavia";
    }
    else
    {
        picName = "";
    }
    QPixmap pix(picName);
    ui->lblLogo->setPixmap(pix);
}


void MainWindow::startSpinner()
{
    ui->lblSpinner->setVisible(true);
    _spinnerMovie->start();
}

void MainWindow::stopSpinner()
{
    _spinnerMovie->stop();
    ui->lblSpinner->setVisible(false);
}
