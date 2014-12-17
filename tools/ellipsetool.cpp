#include "ellipsetool.h"

EllipseTool::EllipseTool(ColorPicker *colorPicker) : Tool(colorPicker, Tool::Ellipse, "Ellipse Tool", ":/tools/ellipse.png")
{
}

void EllipseTool::createToolBar()
{
    this->createBrushSizeSelector();
    this->createOutlineSelector();
}

void EllipseTool::mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(mouse.isButtonDown(Qt::LeftButton)){
        hud->fill(Qt::transparent);

        this->drawEllipse(mouse, hud, selection, useSelection);
    }
}

void EllipseTool::mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    hud->fill(Qt::transparent);

    this->drawEllipse(mouse, layer, selection, useSelection);
}

void EllipseTool::drawEllipse(MouseState mouse, QImage *image, QImage *selection, bool *useSelection)
{
    QImage temp(image->size(), QImage::Format_ARGB32);
    temp.fill(Qt::transparent);
    QPainter painter(&temp);

    QRect ellipseRect = mouse.getClickedRect();

    if("Inside" != m_outlineType->currentText()) {
        int offset = m_brushSize->value();

        if("Middle" == m_outlineType->currentText())
            offset /= 2;

        ellipseRect.setTopLeft(QPoint(ellipseRect.x() - offset, ellipseRect.y() - offset));
        ellipseRect.setSize(QSize(ellipseRect.width() + offset, ellipseRect.height() + offset));
    }

    painter.setPen(this->getFillPen());
    this->drawPerfectEllipse(&painter, ellipseRect, true);

    if(m_brushSize->value() == 1){
        painter.setPen(this->getOutlinePen());
        this->drawPerfectEllipse(&painter, ellipseRect);
    }

    if(m_brushSize->value() >= 2){
        int outlineWidth = m_brushSize->value();

        QPixmap outlinePixmap(image->size());
        outlinePixmap.fill(Qt::transparent);

        QPainter painterTemp(&outlinePixmap);
        painterTemp.setPen(this->getOutlinePen());
        this->drawPerfectEllipse(&painterTemp, ellipseRect, true);
        painterTemp.setPen(Qt::black);
        painterTemp.setCompositionMode(QPainter::CompositionMode_DestinationOut);
        this->drawPerfectEllipse(&painterTemp, ellipseRect.x() + outlineWidth, ellipseRect.y() + outlineWidth, ellipseRect.width() - outlineWidth *2, ellipseRect.height() - outlineWidth *2, true);

        painter.drawPixmap(0, 0, outlinePixmap);
    }

    painter.end();

    this->drawInSelection(image, &temp, selection, useSelection);
}
