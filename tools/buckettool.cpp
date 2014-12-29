#include "buckettool.h"

BucketTool::BucketTool(ColorPicker *colorPicker) : Tool(colorPicker, Tool::BucketTool, "Paint Bucket Tool (G)", ":/tools/bucket.png", "G")
{
    this->setCursor(Bucket);
}

void BucketTool::mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(mouse.button() == Qt::LeftButton){
        hud->fill(Qt::transparent);

        QImage area = getPatternArea(PatternPickerTool::findPattern(*layer, this->getRectPlusOne(mouse)), *layer);
        QImage fill = fillAt(this->getFillBrush(), area, mouse.clickedX(), mouse.clickedY());

        this->drawInSelection(layer, &fill, selection, useSelection);
    }
}

void BucketTool::mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(mouse.isButtonDown(Qt::LeftButton)){
        hud->fill(Qt::transparent);

        QPainter painter(hud);
        painter.setPen(QPen(Qt::NoPen));
        painter.setBrush(QBrush(AREA_BLUE));
        painter.setOpacity(.5);

        painter.drawRect(this->getRectPlusOne(mouse));
    }
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

QImage BucketTool::getPatternArea(QPixmap pattern, QImage source)
{
    QImage image(source.size(), source.format());
    image.fill(Qt::transparent);

    QImage patternImage = pattern.toImage();
    QColor patternMap[pattern.width()][pattern.height()];

    for(int i(0); i < pattern.width(); ++i)
        for(int j(0); j < pattern.height(); ++j)
            patternMap[i][j] = QColor(patternImage.pixel(i, j));

    QPainter painter(&image);
    painter.setPen(QPen(Qt::white));

    for(int i(0); i < source.width(); ++i)
        for(int j(0); j < source.height(); ++j)
            if(QColor(source.pixel(i, j)) == patternMap[i %pattern.width()][j %pattern.height()])
                painter.drawPoint(i, j);

    painter.end();

    QImage element(pattern.width(), pattern.height(), source.format());
    element.fill(Qt::white);

    Canvas::modifySelection(&image, element, Qt::transparent);
    Canvas::modifySelection(&image, element, Qt::white);

    QImage final(image.size(), image.format());
    final.fill(Qt::transparent);

    painter.begin(&final);
    painter.drawImage(0, 0, image);

    return final;
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
