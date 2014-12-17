#ifndef RECTTOOL_H
#define RECTTOOL_H

#include "tool.h"

class RectTool : public Tool
{
public:
    RectTool(ColorPicker *colorPicker);

    void mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);
    void mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);

private:
    void drawRect(MouseState mouse, QImage *image,  QImage *selection, bool *useSelection);

protected:
    void createToolBar();
};

#endif // RECTTOOL_H
