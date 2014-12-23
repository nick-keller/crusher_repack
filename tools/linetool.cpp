#include "linetool.h"

LineTool::LineTool(ColorPicker *colorPicker) : Tool(colorPicker, Tool::Line, "Line Tool (L)", ":/tools/line.png", "L")
{
}

void LineTool::createToolBar()
{
    this->createBrushSizeSelector();
    m_toolbar->addSeparator();
    this->createCapSelector();
    m_brushSize->setMinimum(1);
}

void LineTool::mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(mouse.isButtonDown(Qt::LeftButton)){
        hud->fill(Qt::transparent);

        this->drawLine(mouse, hud, selection, useSelection);
    }
}

void LineTool::mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    hud->fill(Qt::transparent);

    this->drawLine(mouse, layer, selection, useSelection);
}

void LineTool::drawLine(MouseState mouse, QImage *image, QImage *selection, bool *useSelection)
{
    QImage temp(image->size(), image->format());
    temp.fill(Qt::transparent);

    if(m_brushSize->value() == 1){
        QPainter painter(&temp);
        painter.setPen(this->getOutlinePen());
        this->drawPerfectLine(&painter, mouse.getClickedAt(), mouse.getPos());
        painter.end();
    }
    else{
        QPainter *painter = this->getPainter(&temp);
        painter->drawLine(mouse.getClickedAt(), mouse.getPos());
        delete painter;
    }

    this->drawInSelection(image, &temp, selection, useSelection);
}
