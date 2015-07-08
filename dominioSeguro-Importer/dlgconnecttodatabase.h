#ifndef DLGCONNECTTODATABASE_H
#define DLGCONNECTTODATABASE_H

#include <QDialog>

namespace Ui {
class DlgConnectToDatabase;
}

class DlgConnectToDatabase : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConnectToDatabase(QWidget *parent = 0);
    ~DlgConnectToDatabase();

private:
    Ui::DlgConnectToDatabase *ui;
};

#endif // DLGCONNECTTODATABASE_H
