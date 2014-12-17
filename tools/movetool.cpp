#include "movetool.h"

MoveTool::MoveTool(ColorPicker *colorPicker) : Tool(colorPicker, Tool::Move, "Move Tool (V)", ":/tools/move.png", "V")
{
    this->setCursor(MoveCursor);
}

void MoveTool::mousePressEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(mouse.isButtonDown(Qt::LeftButton)){
        if(*useSelection){
            m_movingLayer = QImage(layer->size(), QImage::Format_ARGB32);
            m_movingLayer.fill(Qt::transparent);

            QPainter painter(&m_movingLayer);
            painter.drawImage(0, 0, *selection);
            painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            painter.drawImage(0, 0, *layer);
            painter.end();

            painter.begin(layer);
            painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
            painter.drawImage(0, 0, *selection);
            painter.end();

            painter.begin(hud);
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.drawImage(0, 0, m_movingLayer);
        }
        else{
            m_movingLayer = *layer;
            layer->fill(Qt::transparent);
            QPainter painter(hud);
            painter.drawImage(0, 0, m_movingLayer);
        }
    }
}

void MoveTool::mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(mouse.isButtonDown(Qt::LeftButton)){
        QPoint delta = mouse.getPos() - mouse.getClickedAt();

        hud->fill(Qt::transparent);
        QPainter painter(hud);
        painter.drawImage(delta, m_movingLayer);
    }
    else{
        if(*useSelection && QColor(selection->pixel(mouse.getPos())) == Qt::white)
            this->setCursor(MoveCut);
        else
            this->setCursor(MoveCursor);
    }
}

void MoveTool::mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    hud->fill(Qt::transparent);

    if(!mouse.isButtonDown(Qt::LeftButton)){
        QPoint delta = mouse.getPos() - mouse.getClickedAt();

        QPainter painter(layer);
        painter.drawImage(delta, m_movingLayer);

        if(*useSelection){
            selection->fill(Qt::transparent);

            painter.end();
            painter.begin(&m_movingLayer);
            painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
            painter.setBrush(QBrush(Qt::white));
            painter.setPen(Qt::NoPen);
            painter.drawRect(0, 0, layer->width(), layer->height());
            painter.end();

            painter.begin(selection);
            painter.drawImage(delta, m_movingLayer);
        }
    }
}
