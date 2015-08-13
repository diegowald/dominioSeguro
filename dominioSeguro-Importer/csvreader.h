#ifndef CSVREADER_H
#define CSVREADER_H

#include <QObject>
#include <QMap>

class CSVReader : public QObject
{
    Q_OBJECT
public:
    explicit CSVReader(const QString &filePath,
                       const QString &columnSeparator,
                       int ignoreFirstNLines,
                       const QString &stringDelimiter,
                       QObject *parent = 0);

    void load();
    QStringList headers();
    QStringList record(int index);

signals:

public slots:

private:
    QStringList parse(const QString &row);
private:
    QString _filePath;
    QStringList _headers;
    QMap<int, QStringList> _records;
    QString _columnSeparator;
    int _ignoreFirstNLines;
    QString _stringDelimiter;

};

#endif // CSVREADER_H
