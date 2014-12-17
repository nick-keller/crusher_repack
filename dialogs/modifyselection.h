#ifndef MODIFYSELECTION_H
#define MODIFYSELECTION_H

#include <QtWidgets>
#include "../tools/tool.h"

namespace Ui {
class ModifySelection;
}

class ModifySelection : public QDialog
{
    Q_OBJECT

public:
    explicit ModifySelection(QString operation, QWidget *parent = 0);
    ~ModifySelection();

    QImage getElement();

private:
    Ui::ModifySelection *ui;
};

#endif // MODIFYSELECTION_H
