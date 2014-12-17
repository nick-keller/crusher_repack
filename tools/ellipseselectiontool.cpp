#include "ellipseselectiontool.h"

EllipseSelectionTool::EllipseSelectionTool(ColorPicker *colorPicker) : SelectionTool(colorPicker, Tool::EllipseSelection, "Ellipse selection Tool", ":/tools/select-ellipse.png")
{
}

void EllipseSelectionTool::paintSelection(MouseState mouse, QPainter *painter)
{
    painter->setPen(QPen(painter->brush(), 1));
    this->drawPerfectEllipse(painter, mouse.getClickedRect(), true);
}
