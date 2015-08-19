#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <QObject>
#include <QFile>
#include <QTextStream>

class CSVWriter : public QObject
{
    Q_OBJECT
public:
    explicit CSVWriter(const QString &filePath,
                       const QString &columnSeparator,
                       const QString &stringDelimiter,
                       QObject *parent = 0);

    virtual ~CSVWriter();

    void setHeaders(QStringList &headers);
    void addRecord(QStringList record);

signals:

public slots:

private:
private:
    QString _filePath;
    QStringList _headers;
    QString _columnSeparator;
    int _ignoreFirstNLines;
    QString _stringDelimiter;

    QFile _file;
    QTextStream *_stream;
};

#endif // CSVWRITER_H
