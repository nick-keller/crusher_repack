#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QtWidgets>

namespace Ui {
class ImportDialog;
}

class ImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportDialog(QImage, QWidget *parent = 0);
    ~ImportDialog();

private:
    Ui::ImportDialog *ui;
};

#endif // IMPORTDIALOG_H
