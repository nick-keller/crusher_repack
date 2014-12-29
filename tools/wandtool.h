#ifndef WANDTOOL_H
#define WANDTOOL_H

#include "selectiontool.h"
#include "buckettool.h"

class WandTool : public SelectionTool
{
public:
    WandTool(ColorPicker *colorPicker);

protected:
    void mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);
    void mousePressEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection){}
    void mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);

    void setCorrectCursor();
};

#endif // WANDTOOL_H
