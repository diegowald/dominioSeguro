#include "recordupdater.h"
#include "csvreader.h"
#include "httprequestworker.h"
#include "csvreader.h"
#include <QtConcurrent/QtConcurrent>

RecordUpdater::RecordUpdater(const QString &filename, const QString &columnSeparator, int numLinesToIgnore, const QString &stringDelimiter, QObject *parent) : QObject(parent)
{
    _filename = filename;
    _columnSeparator = columnSeparator;
    _numLinesToIgnore = numLinesToIgnore;
    _stringDelimiter = stringDelimiter;
    _parallelProcesses = 0;
}

void RecordUpdater::run()
{

    QFuture<void> future = QtConcurrent::run(internalUpdateRecords, this);
}

void RecordUpdater::internalUpdateRecords(RecordUpdater *record)
{
    CSVReader csv(record->_filename, record->_columnSeparator,
                  record->_numLinesToIgnore, record->_stringDelimiter);

    csv.load();

    QString url = "http://www.hbobroker.com.ar/smartcard/addrecord";

    int recCount = csv.recordCount();

    for (int i = 0; i < recCount; ++i)
    {
        HttpRequestInput input(url, "POST");

        QStringList rec = csv.record(i);

        input.add_var("dni", rec.at(8));
        input.add_var("dominio", rec.at(9));
        input.add_var("asegurado", rec.at(6));
        input.add_var("cobertura", rec.at(22));
        input.add_var("poliza", rec.at(1));
        input.add_var("vigencia_desde", rec.at(4));
        input.add_var("vigencia_hasta", rec.at(5));
        input.add_var("modelo", rec.at(11));
        input.add_var("anio", rec.at(12));
        input.add_var("chasis", rec.at(15));
        input.add_var("motor", rec.at(14));
        input.add_var("medioPago", "S/D");
        input.add_var("Productor", rec.at(24));

        HttpRequestWorker *worker = new HttpRequestWorker(record);
        connect(worker, &HttpRequestWorker::on_execution_finished, record, &RecordUpdater::on_addRecordFinished);
        while(record->_parallelProcesses > 2)
        {
            QThread::sleep(1);
            //QApplication::processEvents();
        }
        worker->execute(&input);
        record->_parallelProcesses++;
    }
}

void RecordUpdater::on_addRecordFinished(HttpRequestWorker *worker)
{
    if (worker->error_type == QNetworkReply::NoError)
    {
        // communication was successful
        //QByteArray response = worker->response;
        qDebug() << "OK";
    }
    else
    {
        // an error occurred
        QString msg = "Error: " + worker->error_str;
        //QMessageBox::information(this, "", msg);
        qDebug() << msg;
    }

    worker->deleteLater();
    _parallelProcesses--;
}
