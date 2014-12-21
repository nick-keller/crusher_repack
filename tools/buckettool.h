#ifndef BUCKETTOOL_H
#define BUCKETTOOL_H

#include "tool.h"

class BucketTool : public Tool
{
public:
    BucketTool(ColorPicker *colorPicker);

    void mousePressEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);

    static QImage fillAt(QBrush brush, QImage source, int x, int y);

private:
    static void fillAt(QImage *source, QPainter* sourcePainter, QPainter *resultPainter, QRgb target, int x, int y);
};

#endif // BUCKETTOOL_H
