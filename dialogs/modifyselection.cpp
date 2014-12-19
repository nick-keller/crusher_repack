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

int ModifySelection::radius()
{
    return ui->value->value();
}

bool ModifySelection::sharp()
{
    return ui->sharpSelector->isChecked();
}
