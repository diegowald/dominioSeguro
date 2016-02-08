#include "dialoglistadnis.h"
#include "ui_dialoglistadnis.h"
#include <QMovie>

/*ver
http://doc.qt.io/qt-4.8/gestures-overview.html
*/

DialogLIstaDNIS::DialogLIstaDNIS(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLIstaDNIS)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);

    ui->lblSpinner->clear();
    _spinnerMovie = new QMovie(":/img/spinner");
    ui->lblSpinner->setMovie(_spinnerMovie);
    QString s = "QLabel { background-color : transparent; color : darkred; }";
    ui->lblSpinner->setStyleSheet(s);
    ui->lblSpinner->setVisible(false);

}

DialogLIstaDNIS::~DialogLIstaDNIS()
{
    delete ui;
}

void DialogLIstaDNIS::on_btnAddDocument_released()
{
    QString dni = ui->txtDocument->text().trimmed();
    if (dni.length() > 0)
    {
        if (addDNIToList(dni, ""))
        {
            emit requestRegistration(dni);
            startSpinner();
        }
    }
    ui->txtDocument->clear();
}

QMap<QString, QString> DialogLIstaDNIS::dnis()
{
    QMap<QString, QString> dnis;
    for (int i = 0; i < ui->lstDocuments->count(); ++i)
    {
        QListWidgetItem *item = ui->lstDocuments->item(i);
        //dnis.append(item->text());
        QStringList values = item->text().split(" - ");
        dnis[values.at(0)] = values.at(1);
    }
    return dnis;
}

void DialogLIstaDNIS::setDNIs(QMap<QString, QString> &documentos)
{
    ui->lstDocuments->clear();
    foreach (QString dni, documentos.keys())
    {
        addDNIToList(dni, documentos[dni]);
    }
    stopSpinner();
}

bool DialogLIstaDNIS::addDNIToList(const QString &dni, const QString &nombre)
{
    QString format = "%1 - %2";
    QString text = format.arg(dni).arg(nombre);
    QList<QListWidgetItem*> items = ui->lstDocuments->findItems(text, Qt::MatchExactly);
    if (items.count() == 0)
    {
        QListWidgetItem* item = new QListWidgetItem(text);
        item->setData(Qt::UserRole, dni);
        ui->lstDocuments->addItem(item);
        return true;
    }
    return false;
}

void DialogLIstaDNIS::on_lstDocuments_itemDoubleClicked(QListWidgetItem *item)
{
    QString dni = item->data(Qt::UserRole).toString();
    emit removeDNI(dni);
}

void DialogLIstaDNIS::startSpinner()
{
    ui->lblSpinner->setVisible(true);
    _spinnerMovie->start();
}

void DialogLIstaDNIS::stopSpinner()
{
    _spinnerMovie->stop();
    ui->lblSpinner->setVisible(false);
}
