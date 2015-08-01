#ifndef DLGIMPORTCSV_H
#define DLGIMPORTCSV_H

#include <QDialog>

namespace Ui {
class DlgImportCSV;
}

class DlgImportCSV : public QDialog
{
    Q_OBJECT

public:
    explicit DlgImportCSV(QWidget *parent = 0);
    ~DlgImportCSV();

    QString filename() const;
    QString columnSeparator() const;
    QString stringDelimiter() const;
    QString recordSeparator() const;
    int numLinesToIgnore() const;

private slots:
    void on_toolButton_released();

private:
    void parseCSV();

private:
    Ui::DlgImportCSV *ui;
};

#endif // DLGIMPORTCSV_H
