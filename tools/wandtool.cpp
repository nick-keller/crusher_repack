#include "wandtool.h"

WandTool::WandTool(ColorPicker *colorPicker) : SelectionTool(colorPicker, Tool::WandTool, "Magic Wand (W)", ":/tools/wand.png", "W")
{
}

void WandTool::mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(mouse.isButtonDown(Qt::LeftButton)){
        hud->fill(Qt::transparent);

        QPainter painter(hud);
        painter.setPen(QPen(Qt::NoPen));
        painter.setBrush(QBrush(AREA_BLUE));
        painter.setOpacity(.5);

        painter.drawRect(this->getRectPlusOne(mouse));
    }
}

void WandTool::mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    QImage areas = BucketTool::getPatternArea(PatternPickerTool::findPattern(*layer, this->getRectPlusOne(mouse)), *layer);
    QImage area = BucketTool::fillAt(QBrush(Qt::white), areas, mouse.clickedX(), mouse.clickedY());

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
