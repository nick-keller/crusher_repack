#ifndef FILLSELECTOR_H
#define FILLSELECTOR_H

#include <QDialog>

namespace Ui {
class FillSelector;
}

class FillSelector : public QDialog
{
    Q_OBJECT

public:
    explicit FillSelector(QWidget *parent = 0);
    ~FillSelector();

private:
    Ui::FillSelector *ui;
};

#endif // FILLSELECTOR_H
