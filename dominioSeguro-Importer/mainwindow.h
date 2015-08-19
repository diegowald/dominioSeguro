#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "httprequestworker.h"
#include <QMainWindow>
#include "recordupdater.h"
#include <QTemporaryFile>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void uploadData();
    QString transformCSV();

private slots:
    void on_actionUpload_new_data_triggered();

    void on_actionValidate_Users_triggered();

    void on_actionRefresh_triggered();

    void on_statsReceived(HttpRequestWorker* worker);
    void on_updateFinished(HttpRequestWorker *worker);

    void on_updateRecordsFinished();
    void on_uploadCSVfinished(HttpRequestWorker *worker);

private:
    Ui::MainWindow *ui;

    QString _filename;
    QString _columnSeparator;
    QString _stringDelimiter;
    QString _recordSeparator;
    int _numLinesToIgnore;
    RecordUpdater *_updater;
    QTemporaryFile _file;
};

#endif // MAINWINDOW_H
