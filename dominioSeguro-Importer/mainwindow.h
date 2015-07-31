#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionConnect_to_Server_triggered();

    void on_actionUpload_new_data_triggered();

    void on_actionValidate_Users_triggered();

private:
    Ui::MainWindow *ui;

    QString _server;
    QString _database;
    QString _user;
    QString _password;
};

#endif // MAINWINDOW_H
