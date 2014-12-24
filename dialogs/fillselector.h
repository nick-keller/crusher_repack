#ifndef FILLSELECTOR_H
#define FILLSELECTOR_H

#include <QtWidgets>
#include "../widgets/colorpicker.h"

namespace Ui {
class FillSelector;
}

class FillSelector : public QDialog
{
    Q_OBJECT

public:
    explicit FillSelector(ColorPicker *, QWidget *parent = 0);
    ~FillSelector();

    QBrush getBrush();

private:
    Ui::FillSelector *ui;

    ColorPicker *m_colorPicker;
};

#endif // FILLSELECTOR_H
