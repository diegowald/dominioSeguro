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
private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);
    
    void handle_result(HttpRequestWorker *worker);
private:
    Ui::MainWindow *ui;
    QMap<QString, QJsonObject> dominiosAsegurados;
    QString fileLocation;
};

#endif // MAINWINDOW_H
