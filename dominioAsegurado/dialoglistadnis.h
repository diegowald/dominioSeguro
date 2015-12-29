#ifndef DIALOGLISTADNIS_H
#define DIALOGLISTADNIS_H

#include <QDialog>

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

signals:
    void requestRegistration(const QString &DNI);

private:
    bool addDNIToList(const QString &dni, const QString &nombre);

private:
    Ui::DialogLIstaDNIS *ui;
};

#endif // DIALOGLISTADNIS_H
