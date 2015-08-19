#include "csvwriter.h"

CSVWriter::CSVWriter(const QString &filePath,
                     const QString &columnSeparator,
                     const QString &stringDelimiter,
                     QObject *parent) : QObject(parent)
{
    _filePath = filePath;
    _columnSeparator = columnSeparator;
    _stringDelimiter = stringDelimiter;

    _file.setFileName(_filePath);
    if (_file.open(QIODevice::WriteOnly))
    {
        _stream = new QTextStream(&_file);
    }
    else
    {
        _stream = NULL;
    }
}

CSVWriter::~CSVWriter()
{
    if (_stream != NULL)
    {
        _file.close();
    }
    delete _stream;
}

void CSVWriter::setHeaders(QStringList &headers)
{
    addRecord(headers);
}

void CSVWriter::addRecord(QStringList record)
{
    QStringList rec;
    foreach (QString field, record)
    {
        rec.append(QString("%1%2%3").arg(_stringDelimiter).arg(field).arg(_stringDelimiter));
    }
    QString line = rec.join(_columnSeparator.at(0));
    (*_stream) << line << "\n";
}
