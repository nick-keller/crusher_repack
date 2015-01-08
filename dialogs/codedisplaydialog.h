#ifndef CODEDISPLAYDIALOG_H
#define CODEDISPLAYDIALOG_H

#include <QDialog>

namespace Ui {
class CodeDisplayDialog;
}

class CodeDisplayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CodeDisplayDialog(QString code, QWidget *parent = 0);
    ~CodeDisplayDialog();

private:
    Ui::CodeDisplayDialog *ui;
};

#endif // CODEDISPLAYDIALOG_H
