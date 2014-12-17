#include "brushselector.h"
#include "ui_brushselector.h"

BrushSelector::BrushSelector(QWidget *parent) :
    QDialog(parent), m_type(None),
    ui(new Ui::BrushSelector)
{
    ui->setupUi(this);
    QObject::connect(ui->noFill, SIGNAL(clicked()), this, SLOT(setTypeNone()));
    QObject::connect(ui->blackFill, SIGNAL(clicked()), this, SLOT(setTypeBlack()));
    QObject::connect(ui->whiteFill, SIGNAL(clicked()), this, SLOT(setTypeWhite()));
    QObject::connect(ui->patternFill, SIGNAL(clicked()), this, SLOT(setTypePattern()));

    QObject::connect(ui->widthSelector, SIGNAL(valueChanged(int)), ui->patternMaker, SLOT(setWidth(int)));
    QObject::connect(ui->heightSelector, SIGNAL(valueChanged(int)), ui->patternMaker, SLOT(setHeight(int)));
    QObject::connect(ui->colorSelector, SIGNAL(currentIndexChanged(int)), ui->patternMaker, SLOT(setColor(int)));
}

BrushSelector::~BrushSelector()
{
    delete ui;
}

void BrushSelector::setType(BrushSelector::FillType type)
{
    m_type = type;

    switch (type) {
    case None:
        ui->noFill->click();
        break;
    case Black:
        ui->blackFill->click();
        break;
    case White:
        ui->whiteFill->click();
        break;
    case Pattern:
        ui->patternFill->click();
        break;
    }
}

QPixmap BrushSelector::getPattern()
{
    return ui->patternMaker->getPattern();
}

void BrushSelector::setPattern(QPixmap pixmap)
{
    ui->widthSelector->setValue(pixmap.width());
    ui->heightSelector->setValue(pixmap.height());
    ui->patternMaker->setPattern(pixmap);
}
