#include "modifyselection.h"
#include "ui_modifyselection.h"

ModifySelection::ModifySelection(QString operation, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifySelection)
{
    ui->setupUi(this);
    this->setWindowTitle(operation + " Selection");
    ui->label->setText(operation + " By:");
}

ModifySelection::~ModifySelection()
{
    delete ui;
}

QImage ModifySelection::getElement()
{
    int size = ui->value->value() *2 +1;
    QImage element(size, size, QImage::Format_ARGB32);
    element.fill(Qt::black);

}
