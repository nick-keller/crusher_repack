#include "recttool.h"

RectTool::RectTool(ColorPicker *colorPicker) : Tool(colorPicker, Tool::Rect, "Rectangle Tool (U)", ":/tools/rect.png", "U")
{

}

void RectTool::createToolBar()
{
    this->createBrushSizeSelector();
    this->createOutlineSelector();
    m_toolbar->addSeparator();
    this->createAngleSelector();
}

void RectTool::mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(mouse.isButtonDown(Qt::LeftButton)){
        hud->fill(Qt::transparent);

        this->drawRect(mouse, hud, selection, useSelection);
    }
}

void RectTool::mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    hud->fill(Qt::transparent);

    this->drawRect(mouse, layer, selection, useSelection);
}

void RectTool::drawRect(MouseState mouse, QImage *image, QImage *selection, bool *useSelection)
{
    QImage temp(image->size(), image->format());
    temp.fill(Qt::transparent);

    // The target rect
    QRect rect = this->getRectPlusOne(mouse);
    this->constrainRect(mouse, &rect);

    // The outline width
    int outlineWidth = m_colorPicker->getOutlineType() == BrushSelector::None ? 0 : m_brushSize->value();

    if("Inside" == m_outlineType->currentText())
        outlineWidth = std::min(outlineWidth, std::min((rect.width()+1) /2, (rect.height()+1) /2));
    else if("Middle" == m_outlineType->currentText())
        outlineWidth = std::min(outlineWidth, std::min(rect.width(), rect.height()));

    // Outline offset
    int borderOffset, cornerOffset;

    if("Inside" == m_outlineType->currentText()) {
        borderOffset = 0;
        cornerOffset = outlineWidth /2;
    }else if("Middle" == m_outlineType->currentText()) {
        borderOffset = -outlineWidth /2;
        cornerOffset = 0;
    }else {
        borderOffset = -outlineWidth;
        cornerOffset = -outlineWidth /2;
    }

    if("Miter" == m_joinType->currentText())
            cornerOffset = borderOffset;

    QPainter *painter = new QPainter(&temp);
    painter->setPen(Qt::NoPen);

    // Fill
    painter->setBrush(this->getFillBrush());
    painter->drawRect(rect.x() + cornerOffset, rect.y() + cornerOffset, rect.width() - cornerOffset *2, rect.height() - cornerOffset *2);

    // Outline
    painter->setBrush(this->getOutlineBrush());
    painter->drawRect(rect.x() + cornerOffset, rect.y() + borderOffset, rect.width() - cornerOffset *2, outlineWidth);
    painter->drawRect(rect.x() + borderOffset, rect.y() + cornerOffset, outlineWidth, rect.height() - cornerOffset *2);
    painter->drawRect(rect.x() + rect.width() - outlineWidth - borderOffset, rect.y() + cornerOffset, outlineWidth, rect.height() - cornerOffset *2);
    painter->drawRect(rect.x() + cornerOffset, rect.y() + rect.height() - outlineWidth - borderOffset, rect.width() - cornerOffset *2, outlineWidth);

    if("Bevel" == m_joinType->currentText()){
        painter->setPen(QPen(this->getOutlineBrush(), 1));

        for(int i(0); i < outlineWidth /2; ++i){
            this->drawPerfectLine(painter, rect.x() + borderOffset +i, rect.y() + cornerOffset, rect.x() + cornerOffset, rect.y() + borderOffset +i);
            this->drawPerfectLine(painter, rect.x() - cornerOffset + rect.width() -1, rect.y() + borderOffset +i, rect.x() + rect.width() - borderOffset -1 -i, rect.y() + cornerOffset);
            this->drawPerfectLine(painter, rect.x() + borderOffset + i, rect.y() - cornerOffset + rect.height() -1, rect.x() + cornerOffset, rect.y() + rect.height() - borderOffset -1 -i);
            this->drawPerfectLine(painter, rect.x() - cornerOffset + rect.width() -1, rect.y() + rect.height() - borderOffset -1 -i, rect.x() + rect.width() - borderOffset -1 -i, rect.y() - cornerOffset + rect.height() -1);
        }
    }

    if("Round" == m_joinType->currentText()){
        painter->setPen(QPen(this->getOutlineBrush(), 1));

        this->drawPerfectEllipse(painter, rect.x() + borderOffset, rect.y() + borderOffset, outlineWidth, outlineWidth, true);
        this->drawPerfectEllipse(painter, rect.x() + rect.width() - outlineWidth - borderOffset -1, rect.y() + borderOffset, outlineWidth, outlineWidth, true);
        this->drawPerfectEllipse(painter, rect.x() + borderOffset, rect.y() + rect.height() - outlineWidth - borderOffset -1, outlineWidth, outlineWidth, true);
        this->drawPerfectEllipse(painter, rect.x() + rect.width() - outlineWidth - borderOffset -1, rect.y() + rect.height() - outlineWidth - borderOffset -1, outlineWidth, outlineWidth, true);
    }

    delete painter;

    this->drawInSelection(image, &temp, selection, useSelection);
}
