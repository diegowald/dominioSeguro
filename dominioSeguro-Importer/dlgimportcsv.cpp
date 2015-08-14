#include "dlgimportcsv.h"
#include "ui_dlgimportcsv.h"

#include <QFileDialog>
#include <QTextStream>
#include <QDebug>

#include "csvreader.h"


DlgImportCSV::DlgImportCSV(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgImportCSV)
{
    ui->setupUi(this);
    ui->txtFilename->clear();
    ui->txtColumnSeparator->setText(",");
    ui->txtIgnoreFirstNLines->setText("0");
    ui->txtRecordSeparator->setText("\\n");
    ui->txtStringDelimiter->setText("\"");
}

DlgImportCSV::~DlgImportCSV()
{
    delete ui;
}

QString DlgImportCSV::filename() const
{
    return ui->txtFilename->text();
}

QString DlgImportCSV::columnSeparator() const
{
    return ui->txtColumnSeparator->text();
}

QString DlgImportCSV::stringDelimiter() const
{
    return ui->txtStringDelimiter->text();
}

QString DlgImportCSV::recordSeparator() const
{
    return ui->txtRecordSeparator->text();
}

int DlgImportCSV::numLinesToIgnore() const
{
    return ui->txtIgnoreFirstNLines->text().toInt();
}

void DlgImportCSV::on_toolButton_released()
{
    QString file = QFileDialog::getOpenFileName(this, "Open CSV", "./", tr("CSV-Files (*.csv);;All Files (*)"));
    if (file.length() > 0)
    {
        ui->txtFilename->setText(file);
        parseCSV();
    }
}

void DlgImportCSV::parseCSV()
{
    ui->tblCSV->setRowCount(0);
    ui->tblCSV->setColumnCount(0);


    QFile file(filename());
    CSVReader csv(filename(), ui->txtColumnSeparator->text(),
                  ui->txtIgnoreFirstNLines->text().toInt(),
                  ui->txtStringDelimiter->text());

    csv.load();
    qDebug() << csv.headers();

    ui->tblCSV->setColumnCount(csv.headers().count());
    ui->tblCSV->setHorizontalHeaderLabels(csv.headers());

    int recCount = csv.recordCount();
    ui->tblCSV->setRowCount(recCount);

    for (int i = 0; i < recCount; ++i)
    {
        QStringList rec = csv.record(i);
        for (int j = 0; j < rec.count(); ++j)
        {
            ui->tblCSV->setItem(i, j, new QTableWidgetItem(rec.at(j)));
        }
    }
}

void DlgImportCSV::on_btnPreview_released()
{
    parseCSV();
}
