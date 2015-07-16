#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "httprequestworker.h"
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>

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
    void loadJson(QJsonDocument &jsonDoc);
    void loadJsonSettings(QJsonDocument &jsonDoc);
private slots:

    void on_comboBox_currentIndexChanged(const QString &arg1);
    
    void handle_result(HttpRequestWorker *worker);
    void on_btnUpdate_clicked();

    void on_btnCallForCrane_clicked();

    void on_btnCallForCrash_clicked();

    void on_btnCallForInformation_clicked();

private:
    Ui::MainWindow *ui;
    QMap<QString, QJsonObject> dominiosAsegurados;
    QString _fileDataLocation;
    QString _settingsLocation;
    QString _dniAsociado;
};

#endif // MAINWINDOW_H
