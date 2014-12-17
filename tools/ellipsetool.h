#ifndef ELLIPSETOOL_H
#define ELLIPSETOOL_H

#include "tool.h"

class EllipseTool : public Tool
{
public:
    EllipseTool(ColorPicker *colorPicker);

    void mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);
    void mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);

private:
    void drawEllipse(MouseState mouse, QImage *image, QImage *selection, bool *useSelection);

protected:
    void createToolBar();
};

#endif // ELLIPSETOOL_H
