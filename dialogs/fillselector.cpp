#include "fillselector.h"
#include "ui_fillselector.h"

FillSelector::FillSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FillSelector)
{
    ui->setupUi(this);
}

FillSelector::~FillSelector()
{
    delete ui;
}
