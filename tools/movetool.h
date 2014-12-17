#ifndef MOVETOOL_H
#define MOVETOOL_H

#include "tool.h"

class MoveTool : public Tool
{
public:
    MoveTool(ColorPicker *colorPicker);

    void mousePressEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);
    void mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);
    void mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);

private:
    QImage m_movingLayer;
};

#endif // MOVETOOL_H
