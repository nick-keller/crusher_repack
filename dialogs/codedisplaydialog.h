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
    explicit CodeDisplayDialog(QString code, int rects, int lines, int dots, QWidget *parent = 0);
    ~CodeDisplayDialog();

public slots:
    void on_copyBtn_clicked();

private:
    Ui::CodeDisplayDialog *ui;
};

#endif // CODEDISPLAYDIALOG_H
