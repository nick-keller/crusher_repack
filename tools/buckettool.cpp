#include "buckettool.h"

BucketTool::BucketTool(ColorPicker *colorPicker) : Tool(colorPicker, Tool::BucketTool, "Paint Bucket Tool (G)", ":/tools/bucket.png", "G")
{
    this->setCursor(Bucket);
}

void BucketTool::mousePressEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    QImage fill = fillAt(this->getFillBrush(), *layer, mouse.x(), mouse.y());

    this->drawInSelection(layer, &fill, selection, useSelection);
}

QImage BucketTool::fillAt(QBrush brush, QImage source, int x, int y)
{
    QImage result(source.width(), source.height(), source.format());
    result.fill(Qt::transparent);

    QPainter resultPainter(&result);
    QPainter sourcePainter(&source);
    resultPainter.setPen(QPen(brush, 1));
    sourcePainter.setPen(QPen(Qt::red));

    fillAt(&source, &sourcePainter, &resultPainter, source.pixel(x, y), x, y);

    return result;
}

void BucketTool::fillAt(QImage *source, QPainter *sourcePainter, QPainter *resultPainter, QRgb target, int x, int y)
{
    if(x < 0 || y < 0 || x == source->width() || y == source->height())
        return;

    QRgb color = source->pixel(x, y);

    if(color != target)
        return;

    sourcePainter->drawPoint(x, y);
    resultPainter->drawPoint(x, y);

    fillAt(source, sourcePainter, resultPainter, target, x+1, y);
    fillAt(source, sourcePainter, resultPainter, target, x-1, y);
    fillAt(source, sourcePainter, resultPainter, target, x, y+1);
    fillAt(source, sourcePainter, resultPainter, target, x, y-1);
}
