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

private:
    Ui::DlgImportCSV *ui;
};

#endif // DLGIMPORTCSV_H
