#include "wandtool.h"

WandTool::WandTool(ColorPicker *colorPicker) : SelectionTool(colorPicker, Tool::WandTool, "Magic Wand (W)", ":/tools/wand.png", "W")
{
}

void WandTool::mousePressEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    QImage area = BucketTool::fillAt(QBrush(Qt::white), *layer, mouse.x(), mouse.y());

    if(*useSelection == false)
        selection->fill(Qt::transparent);

    QPainter painter(selection);

    if(m_selectionMode == "only")
        painter.setCompositionMode(QPainter::CompositionMode_Source);
    if(m_selectionMode == "not")
        painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    if(m_selectionMode == "and")
        painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);

    painter.drawImage(0, 0, area);

    *useSelection = true;
    emit selectionChanged(true);
}

void WandTool::setCorrectCursor()
{
    this->setCursor(Wand);
}
