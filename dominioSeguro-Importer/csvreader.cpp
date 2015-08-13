#include "csvreader.h"
#include <QFile>
#include <QTextStream>


CSVReader::CSVReader(const QString &filePath, const QString &columnSeparator, int ignoreFirstNLines, const QString &stringDelimiter, QObject *parent) : QObject(parent)
{
    _filePath = filePath;
    _headers.clear();
    _records.clear();
    _columnSeparator = columnSeparator;
    _ignoreFirstNLines = ignoreFirstNLines;
    _stringDelimiter = stringDelimiter;

}

void CSVReader::load()
{

    QFile file(_filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);

        int currentLine = 0;
        int pos = -1;
        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            if (currentLine > _ignoreFirstNLines)
            {
                QStringList row = parse(line);
                if (pos == -1)
                {
                    _headers = row;
                }
                else
                {
                    _records[pos] = row;
                }
                pos++;
            }
            currentLine++;
        }
        file.close();
    }
}

QStringList CSVReader::headers()
{
    return _headers;
}

QStringList CSVReader::record(int index)
{
    if (_records.contains(index))
        return _records[index];
    else
        return QStringList();
}

QStringList CSVReader::parse(const QString &row)
{
    QStringList res;
    QStringList partialRes = row.split(_columnSeparator);
    foreach (QString s, partialRes)
    {
        QString r = s;
        if (r.startsWith(_stringDelimiter))
            r = r.remove(0, _stringDelimiter.length());
        if (r.endsWith(_stringDelimiter))
            r.chop(_stringDelimiter.length());
        res.append(r);
    }
    return res;
}
