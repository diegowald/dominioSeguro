#ifndef RECORDUPDATER_H
#define RECORDUPDATER_H

#include <QObject>
#include "httprequestworker.h"
#include <QAtomicInt>

class RecordUpdater : public QObject
{
    Q_OBJECT
public:
    explicit RecordUpdater(const QString &filename, const QString &columnSeparator,
                           int numLinesToIgnore,
                           const QString &stringDelimiter, QObject *parent = 0);

    void run();
signals:
    void processFinished();

private slots:
    void on_addRecordFinished(HttpRequestWorker *worker);

private:
    static void internalUpdateRecords(RecordUpdater *record);

private:
    QString _filename;
    QString _columnSeparator;
    int _numLinesToIgnore;
    QString _stringDelimiter;
    QAtomicInt _parallelProcesses;
};

#endif // RECORDUPDATER_H
