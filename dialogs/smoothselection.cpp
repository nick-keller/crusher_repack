#include "smoothselection.h"
#include "ui_smoothselection.h"

SmoothSelection::SmoothSelection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SmoothSelection)
{
    ui->setupUi(this);
}

SmoothSelection::~SmoothSelection()
{
    delete ui;
}
