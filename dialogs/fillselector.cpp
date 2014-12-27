#include "fillselector.h"
#include "ui_fillselector.h"

FillSelector::FillSelector(ColorPicker *colorPicker, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FillSelector),
    m_colorPicker(colorPicker)
{
    ui->setupUi(this);
}

FillSelector::~FillSelector()
{
    delete ui;
}

QBrush FillSelector::getBrush()
{
    if(ui->selector->currentText() == "Black")
        return QBrush(BLACK);
    if(ui->selector->currentText() == "White")
        return QBrush(WHITE);
    if(ui->selector->currentText() == "Fill color")
        return m_colorPicker->getFillBrush();
    if(ui->selector->currentText() == "Outline color")
        return m_colorPicker->getOutlineBrush();
}
