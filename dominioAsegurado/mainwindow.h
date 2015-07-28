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

    void registrar();
private slots:

    void on_comboBox_currentIndexChanged(const QString &arg1);
    
    void handle_resultUpdate(HttpRequestWorker *worker);
    void handle_resultRegistration(HttpRequestWorker *worker);
    void on_btnGetInformationUpdates_pressed();
    void on_btnCrane_pressed();
    void on_btnCrash_pressed();
    void on_btnFeedback_pressed();
    void on_btnInformation_pressed();

private:
    Ui::MainWindow *ui;
    QMap<QString, QJsonObject> dominiosAsegurados;
    QString _fileDataLocation;
    QString _settingsLocation;
    QString _dniAsociado;
};

#endif // MAINWINDOW_H
