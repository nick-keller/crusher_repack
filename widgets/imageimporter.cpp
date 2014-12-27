#include "imageimporter.h"

ImageImporter::ImageImporter(QWidget *parent) :
    QWidget(parent), m_showOriginal(false), m_dithering(false), m_ditheringLevel(3)
{
    this->setFixedSize(258, 130);
    this->createBrushes();
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

void ImageImporter::createBrushes()
{
    QImage gradientPatterns(":/icons/gradient.png");
    QPixmap texture(4, 4);
    QPainter painter(&gradientPatterns);

    for(int i(0); i < gradientPatterns.width(); ++i){
        for(int j(0); j < gradientPatterns.height(); ++j){
            QColor color = QColor(gradientPatterns.pixel(i, j));

            if(color == Qt::white)
                painter.setPen(QPen(WHITE));
            else
                painter.setPen(QPen(BLACK));

            painter.drawPoint(i, j);
        }
    }

    painter.end();
    painter.begin(&texture);

    for(int i(0); i < gradientPatterns.width()/4; ++i){
        painter.drawImage(-i*4, 0, gradientPatterns);
        m_brushes.append(QBrush(texture));
    }
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

            if(m_dithering){
                float val = 1- level / 255.f;
                painter.setPen(QPen(m_brushes[(int) round(val * std::pow(2, m_ditheringLevel -1)) * std::pow(2, 5-m_ditheringLevel)], 1));
            }
            else{
                if(qrand() % 255 < level)
                    painter.setPen(QPen(WHITE));
                else
                    painter.setPen(QPen(BLACK));
            }

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

void ImageImporter::setDithering(bool dithering)
{
    m_dithering = dithering;
    this->setImage(m_original);
}

void ImageImporter::setDitheringLevel(int level)
{
    m_ditheringLevel = level;
    this->setImage(m_original);
}
