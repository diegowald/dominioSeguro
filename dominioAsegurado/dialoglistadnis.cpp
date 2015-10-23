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
        QList<QListWidgetItem*> items = ui->lstDocuments->findItems(dni, Qt::MatchExactly);
        if (items.count() == 0)
        {
            QListWidgetItem* item = new QListWidgetItem(ui->txtDocument->text().trimmed());
            ui->lstDocuments->addItem(item);
            emit requestRegistration(dni);
        }
    }
}
