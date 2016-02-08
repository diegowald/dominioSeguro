#ifndef DIALOGLISTADNIS_H
#define DIALOGLISTADNIS_H

#include <QDialog>
#include <QListWidgetItem>


namespace Ui {
class DialogLIstaDNIS;
}

class DialogLIstaDNIS : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLIstaDNIS(QWidget *parent = 0);
    ~DialogLIstaDNIS();

    QMap<QString, QString> dnis();
    void setDNIs(QMap<QString, QString> &documentos);
private slots:
    void on_btnAddDocument_released();

    void on_lstDocuments_itemDoubleClicked(QListWidgetItem *item);

signals:
    void requestRegistration(const QString &DNI);
    void removeDNI(const QString &DNI);

private:
    bool addDNIToList(const QString &dni, const QString &nombre);
    void startSpinner();
    void stopSpinner();

private:
    Ui::DialogLIstaDNIS *ui;
    QMovie* _spinnerMovie;
};

#endif // DIALOGLISTADNIS_H
