#include "brushtool.h"

BrushTool::BrushTool(ColorPicker *colorPicker) : Tool(colorPicker, Tool::Brush, "Brush Tool (B)", ":/tools/brush.png", "B")
{
}

void BrushTool::mousePressEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(mouse.button() == Qt::LeftButton){
        m_last = mouse.getPos();
    }
}

void BrushTool::mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(mouse.isButtonDown(Qt::LeftButton)){
        hud->fill(Qt::transparent);

        QImage temp(layer->size(), layer->format());
        temp.fill(Qt::transparent);

        QPainter *painter = this->getPainter(&temp);

        if(m_brushSize->value() == 1)
            Tool::drawPerfectLine(painter, mouse.getPos(), m_last);
        else
            painter->drawLine(mouse.getPos(), m_last);

        delete painter;

        if(QVector2D(mouse.getPos() - m_last).lengthSquared() >= (m_brushSize->value() + 2) * m_brushSize->value()){
            m_last = mouse.getPos();

            this->drawInSelection(layer, &temp, selection, useSelection);
        }
        else{
            this->drawInSelection(hud, &temp, selection, useSelection);
        }
    }
}

void BrushTool::mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(mouse.button() == Qt::LeftButton){
        hud->fill(Qt::transparent);

        QImage temp(layer->size(), layer->format());
        temp.fill(Qt::transparent);

        QPainter *painter = this->getPainter(&temp);

        if(m_brushSize->value() == 1)
            Tool::drawPerfectLine(painter, mouse.getPos(), m_last);
        else
            painter->drawLine(mouse.getPos(), m_last);

        delete painter;

        this->drawInSelection(layer, &temp, selection, useSelection);
    }
}

void BrushTool::createToolBar()
{
    this->createBrushSizeSelector();
    m_brushSize->setMinimum(1);
}

QPainter *BrushTool::getPainter(QPaintDevice *device)
{
    QPainter *painter = Tool::getPainter(device);
    QPen pen = painter->pen();
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);

    return painter;
}
