#ifndef BRUSHTOOL_H
#define BRUSHTOOL_H

#include "tool.h"

class BrushTool : public Tool
{
public:
    BrushTool(ColorPicker *colorPicker);

    void mousePressEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);
    void mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);
    void mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);

protected:
    void createToolBar();
    QPainter *getPainter(QPaintDevice *device);

private:
    QPoint m_last;
};

#endif // BRUSHTOOL_H
