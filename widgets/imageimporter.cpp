#include "imageimporter.h"

ImageImporter::ImageImporter(QWidget *parent) :
    QWidget(parent), m_showOriginal(false)
{
    this->setFixedSize(258, 130);
}

void ImageImporter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if(m_showOriginal)
        painter.drawImage(QRect(1, 1, this->width() -2, this->height() -2), m_original);
    else
        painter.drawImage(QRect(1, 1, this->width() -2, this->height() -2), m_result);

    painter.setPen(QPen(QColor(40, 40, 40)));
    painter.drawRect(0, 0, this->width() -1, this->height() -1);
}

void ImageImporter::setImage(QImage image)
{
    m_original = image;
    m_result = image;

    qsrand(0);

    QPainter painter(&m_result);

    for(int i(0); i < image.width(); ++i){
        for(int j(0); j < image.height(); ++j){
            int level = qRed(image.pixel(i, j));

            if(qrand() % 255 < level)
                painter.setPen(QPen(WHITE));
            else
                painter.setPen(QPen(BLACK));

            painter.drawPoint(i, j);
        }
    }

    m_showOriginal = false;
    update();
}

void ImageImporter::setPreview(QImage image)
{
    m_original = image;
    m_showOriginal = true;
    update();
}
