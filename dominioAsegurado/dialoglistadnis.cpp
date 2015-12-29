#include "dialoglistadnis.h"
#include "ui_dialoglistadnis.h"

DialogLIstaDNIS::DialogLIstaDNIS(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLIstaDNIS)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
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
        }
    }
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
