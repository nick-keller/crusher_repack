#ifndef LINETOOL_H
#define LINETOOL_H

#include "tool.h"

class LineTool : public Tool
{
public:
    LineTool(ColorPicker *colorPicker);

    void mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);
    void mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);

private:
    void drawLine(MouseState mouse, QImage *image, QImage *selection, bool *useSelection);

protected:
    void createToolBar();
};

#endif // LINETOOL_H
