#include "marqueetool.h"

MarqueeTool::MarqueeTool(ColorPicker *colorPicker) : SelectionTool(colorPicker, Tool::Marquee, "Marquee Tool (M)", ":/tools/marquee.png", "M")
{
}

void MarqueeTool::paintSelection(MouseState mouse, QPainter *painter)
{
    QRect rect = this->getRectPlusOne(mouse);
    this->constrainRect(mouse, &rect);
    painter->drawRect(rect);
}
