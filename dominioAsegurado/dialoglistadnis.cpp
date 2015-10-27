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
        if (addDNIToList(dni))
        {
            emit requestRegistration(dni);
        }
    }
}

QStringList DialogLIstaDNIS::dnis()
{
    QStringList dnis;
    for (int i = 0; i < ui->lstDocuments->count(); ++i)
    {
        QListWidgetItem *item = ui->lstDocuments->item(i);
        dnis.append(item->text());
    }
    return dnis;
}

void DialogLIstaDNIS::setDNIs(QStringList &documentos)
{
    ui->lstDocuments->clear();
    foreach (QString dni, documentos)
    {
        addDNIToList(dni);
    }
}

bool DialogLIstaDNIS::addDNIToList(const QString &dni)
{
    QList<QListWidgetItem*> items = ui->lstDocuments->findItems(dni, Qt::MatchExactly);
    if (items.count() == 0)
    {
        QListWidgetItem* item = new QListWidgetItem(dni);
        ui->lstDocuments->addItem(item);
        return true;
    }
    return false;
}
