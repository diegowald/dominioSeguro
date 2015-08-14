#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "httprequestworker.h"
#include <QMainWindow>
#include "recordupdater.h"

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
    void uploadData2();

private slots:
    void on_actionConnect_to_Server_triggered();

    void on_actionUpload_new_data_triggered();

    void on_actionValidate_Users_triggered();

    void on_actionRefresh_triggered();

    void on_statsReceived(HttpRequestWorker* worker);
    void on_updateFinished(HttpRequestWorker *worker);

    void on_updateRecordsFinished();

private:
    Ui::MainWindow *ui;

    QString _server;
    QString _database;
    QString _user;
    QString _password;

    QString _filename;
    QString _columnSeparator;
    QString _stringDelimiter;
    QString _recordSeparator;
    int _numLinesToIgnore;
    RecordUpdater *_updater;
};

#endif // MAINWINDOW_H
