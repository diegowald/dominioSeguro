#ifndef DLGNOTIMPLEMENTED_H
#define DLGNOTIMPLEMENTED_H

#include <QDialog>

namespace Ui {
class DlgNotImplemented;
}

class DlgNotImplemented : public QDialog
{
    Q_OBJECT

public:
    explicit DlgNotImplemented(QWidget *parent = 0);
    ~DlgNotImplemented();

private:
    Ui::DlgNotImplemented *ui;
};

#endif // DLGNOTIMPLEMENTED_H
