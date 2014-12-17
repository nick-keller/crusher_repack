#include "marqueetool.h"

MarqueeTool::MarqueeTool(ColorPicker *colorPicker) : SelectionTool(colorPicker, Tool::Marquee, "Marquee Tool (M)", ":/tools/marquee.png", "M")
{
}

void MarqueeTool::paintSelection(MouseState mouse, QPainter *painter)
{
    painter->drawRect(this->getRectPlusOne(mouse));
}
