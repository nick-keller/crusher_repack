#include "colorpicker.h"

ColorPicker::ColorPicker(QWidget *parent) :
    QWidget(parent),
    m_fillRect(11, 11, 22, 17),
    m_outlineRect(2, 2, 22, 17),
    m_swapRect(25, 0, 10, 10),
    m_initRect(0, 20, 10, 10),
    m_noFill(":/icons/nofill.png"),
    m_blackFill(":/icons/black.png"),
    m_whiteFill(":/icons/white.png"),
    m_patternFill(":/icons/pattern.png"),
    m_background(":/icons/color-picker.png")
{
    this->setFixedSize(35, 31);
    m_outlineSelector.setType(BrushSelector::Black);
    m_fillSelector.setType(BrushSelector::White);

    QAction *swapAction = new QAction(this);
    QAction *initAction = new QAction(this);
    swapAction->setShortcut(QKeySequence("X"));
    initAction->setShortcut(QKeySequence("D"));

    QObject::connect(swapAction, SIGNAL(triggered()), this, SLOT(swap()));
    QObject::connect(initAction, SIGNAL(triggered()), this, SLOT(init()));

    this->addAction(swapAction);
    this->addAction(initAction);
}

void ColorPicker::paintEvent(QPaintEvent *event)
{
    QPainter painter(&m_background);
    painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    painter.drawPixmap(m_outlineRect.topLeft(), this->getPixmap(m_outlineSelector.getType()));
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawPixmap(m_fillRect.topLeft(), this->getPixmap(m_fillSelector.getType()));

    painter.setPen(QPen(QColor(34, 34, 34)));
    painter.drawLine(10, 13, 7, 13);
    painter.drawLine(7, 13, 7, 7);
    painter.drawLine(7, 7, 18, 7);
    painter.drawLine(18, 7, 18, 10);

    QPainter mainPainter(this);
    mainPainter.drawPixmap(0, 0, m_background);
}

void ColorPicker::mousePressEvent(QMouseEvent *event)
{
    if(m_fillRect.contains(event->pos()))
        this->prompt(&m_fillSelector);

    else if(m_outlineRect.contains(event->pos()))
        this->prompt(&m_outlineSelector);

    if(m_swapRect.contains(event->pos()))
        this->swap();

    if(m_initRect.contains(event->pos()))
        this->init();
}

QPixmap &ColorPicker::getPixmap(BrushSelector::FillType type)
{
    switch (type) {
    case BrushSelector::None:
        return m_noFill;
    case BrushSelector::Black:
        return m_blackFill;
    case BrushSelector::White:
        return m_whiteFill;
    case BrushSelector::Pattern:
        return m_patternFill;
    }
}

void ColorPicker::swap()
{
    BrushSelector::FillType type = m_outlineSelector.getType();
    m_outlineSelector.setType(m_fillSelector.getType());
    m_fillSelector.setType(type);

    QPixmap pattern = m_outlineSelector.getPattern();
    m_outlineSelector.setPattern(m_fillSelector.getPattern());
    m_fillSelector.setPattern(pattern);

    update();
}

void ColorPicker::init()
{
    m_outlineSelector.setType(BrushSelector::Black);
    m_fillSelector.setType(BrushSelector::White);

    update();
}

void ColorPicker::prompt(BrushSelector *selector)
{
    BrushSelector::FillType type    = selector->getType();
    QPixmap pattern                 = selector->getPattern();

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(selector->exec());

    if(code == QDialog::Rejected){
        selector->setType(type);
        selector->setPattern(pattern);
    }
}
