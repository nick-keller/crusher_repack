#ifndef BUCKETTOOL_H
#define BUCKETTOOL_H

#include "tool.h"
#include "patternpickertool.h"
#include "../widgets/canvas.h"

class BucketTool : public Tool
{
public:
    BucketTool(ColorPicker *colorPicker);

    void mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);
    void mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);

    static QImage fillAt(QBrush brush, QImage source, int x, int y);
    static QImage getPatternArea(QPixmap pattern, QImage source);

private:
    static void fillAt(QImage *source, QPainter* sourcePainter, QPainter *resultPainter, QRgb target, int x, int y);
};

#endif // BUCKETTOOL_H
