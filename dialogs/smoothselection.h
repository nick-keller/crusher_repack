#ifndef SMOOTHSELECTION_H
#define SMOOTHSELECTION_H

#include <QDialog>

namespace Ui {
class SmoothSelection;
}

class SmoothSelection : public QDialog
{
    Q_OBJECT

public:
    explicit SmoothSelection(QWidget *parent = 0);
    ~SmoothSelection();

private:
    Ui::SmoothSelection *ui;
};

#endif // SMOOTHSELECTION_H
