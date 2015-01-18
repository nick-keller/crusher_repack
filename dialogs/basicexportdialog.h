#ifndef BASICEXPORTDIALOG_H
#define BASICEXPORTDIALOG_H

#include <QDialog>

namespace Ui {
class BasicExportDialog;
}

class BasicExportDialog : public QDialog
{
    Q_OBJECT

public:
    enum Function {PxlOn, PlotOn, FLine, ForLoop, DrawStat};
    enum Strategy {OutOfRangePoint, MultipleDrawStat};

    explicit BasicExportDialog(QWidget *parent = 0);
    ~BasicExportDialog();

    Function dotsFunction();
    int dotsListX();
    int dotsListY();

    Function rectsFunction();
    QString rectsLoopVar();
    int rectsListX();
    int rectsListY();

    Function linesFunction();
    int linesListX();
    int linesListY();

    Strategy drawStatStrategy();

public slots:
    void on_globalDrawStat_clicked(bool);
    void on_globalListX_currentIndexChanged(int);
    void on_globalListY_currentIndexChanged(int);
    void customDrawStatListX(int i);
    void customDrawStatListY(int i);
    void customDrawStat(bool);

private:
    Ui::BasicExportDialog *ui;
};

#endif // BASICEXPORTDIALOG_H
