#include "imagecropper.h"

ImageCropper::ImageCropper(QWidget *parent) :
    QWidget(parent), m_crop(true), m_stretch(true)
{
    this->setFixedSize(SIZE, SIZE);
    this->setCursor(Qt::OpenHandCursor);
}

void ImageCropper::setImage(QImage image)
{
    m_image = image;

    m_imageRect = QRect(1, 1, m_image.width(), m_image.height());
    m_imageRect.setSize(m_imageRect.size() * (SIZE -2) / std::max(m_imageRect.width(), m_imageRect.height()));
    m_imageRect.moveLeft((SIZE - m_imageRect.width()) /2);
    m_imageRect.moveTop((SIZE - m_imageRect.height()) /2);

    m_croppingZone = QRect(1, 1, SIZE -2, (SIZE -2) /2);

    if(m_croppingZone.width() > m_imageRect.width())
        m_croppingZone.setSize(m_croppingZone.size() * m_imageRect.width() / m_croppingZone.width());

    if(m_croppingZone.height() > m_imageRect.height())
        m_croppingZone.setSize(m_croppingZone.size() * m_imageRect.height() / m_croppingZone.height());

    m_croppingZone.moveLeft((SIZE - m_croppingZone.width()) /2);
    m_croppingZone.moveTop((SIZE - m_croppingZone.height()) /2);

    emit imageCropped(this->getCroppedImage());
}

void ImageCropper::paintEvent(QPaintEvent *event)
{

    // Background
    QPainter painter(this);
    painter.fillRect(0, 0, SIZE, SIZE, QColor(147, 147, 147));

    // Image
    painter.drawImage(m_imageRect, m_image);

    if(m_crop){
        // Cropping zone
        painter.fillRect(0, 0, SIZE, m_croppingZone.y(), QColor(0, 0, 0, 128));
        painter.fillRect(0, m_croppingZone.y(), m_croppingZone.x(), m_croppingZone.height(), QColor(0, 0, 0, 128));
        painter.fillRect(m_croppingZone.right(), m_croppingZone.y(), SIZE, m_croppingZone.height(), QColor(0, 0, 0, 128));
        painter.fillRect(0, m_croppingZone.bottom() +1, SIZE, SIZE, QColor(0, 0, 0, 128));

        painter.setBrush(QBrush(Qt::NoBrush));
        painter.setPen(QPen(Qt::red));
        painter.drawRect(m_croppingZone.x(), m_croppingZone.y(), m_croppingZone.width() -1, m_croppingZone.height() -1);
    }

    // Border
    painter.setPen(QPen(QColor(40, 40, 40)));
    painter.setBrush(QBrush(Qt::NoBrush));
    painter.drawRect(0, 0, this->width() -1, this->height() -1);
}

void ImageCropper::mousePressEvent(QMouseEvent *event)
{
    m_mouse.handleEvent(event);

    if(m_mouse.button() == Qt::LeftButton && m_crop){
        this->setCursor(Qt::ClosedHandCursor);
        m_initPos = m_croppingZone.topLeft();
    }
}

void ImageCropper::mouseMoveEvent(QMouseEvent *event)
{
    m_mouse.handleEvent(event);

    if(m_mouse.isButtonDown(Qt::LeftButton) && m_crop){
        m_croppingZone.moveTopLeft(m_initPos + m_mouse.delta());
        m_croppingZone.moveLeft(std::max(m_imageRect.left(), m_croppingZone.left()));
        m_croppingZone.moveRight(std::min(m_imageRect.right(), m_croppingZone.right()));
        m_croppingZone.moveTop(std::max(m_imageRect.top(), m_croppingZone.top()));
        m_croppingZone.moveBottom(std::min(m_imageRect.bottom(), m_croppingZone.bottom()));
        update();
        emit imageCropping(this->getCroppedImage(false));
    }
}

void ImageCropper::mouseReleaseEvent(QMouseEvent *event)
{
    m_mouse.handleEvent(event);

    if(m_mouse.button() == Qt::LeftButton && m_crop){
        this->setCursor(Qt::OpenHandCursor);
        emit imageCropped(this->getCroppedImage());
    }
}

QImage ImageCropper::getCroppedImage(bool grayscale)
{
    QImage image(SCREEN_WIDTH, SCREEN_HEIGHT, QImage::Format_ARGB32);
    image.fill(Qt::white);
    QPainter painter(&image);

    if(m_crop){
        float fact = (float) m_image.width() / (float) m_imageRect.width();
        if(m_image.height() < m_image.width())
            fact = (float) m_image.height() / (float) m_imageRect.height();

        painter.drawImage(image.rect(), m_image, QRect(
                              (m_croppingZone.x() - m_imageRect.x()) * fact,
                              (m_croppingZone.y() - m_imageRect.y()) * fact,
                              m_croppingZone.width() * fact,
                              m_croppingZone.height() * fact));
    }
    else{
        if(m_fit){
            QSize size = m_image.size();

            size.scale(image.size(), Qt::KeepAspectRatio);

            painter.drawImage(QRect((image.width() - size.width()) /2, (image.height() - size.height()) /2, size.width(), size.height()), m_image);
        }
        else
            painter.drawImage(image.rect(), m_image);
    }

    if(grayscale){
        int min = 255, max = 0;

        // Grayscale
        for(int i(0); i < image.width(); ++i){
            for(int j(0); j < image.height(); ++j){
                int color = qGray(image.pixel(i, j));

                min = std::min(min, color);
                max = std::max(max, color);

                painter.setPen(QPen(QColor(color, color, color)));
                painter.drawPoint(i, j);
            }
        }

        if(m_stretch){
            // Stretch histogram
            if(max != min){
                float scale = 255 / (float) (max - min);

                for(int i(0); i < image.width(); ++i){
                    for(int j(0); j < image.height(); ++j){
                        int color = (qGray(image.pixel(i, j)) - min) * scale;

                        painter.setPen(QPen(QColor(color, color, color)));
                        painter.drawPoint(i, j);
                    }
                }
            }
        }
    }

    return image;
}

void ImageCropper::setCroppingCrop()
{
    m_crop = true;

    this->setCursor(Qt::OpenHandCursor);

    update();
    emit imageCropped(this->getCroppedImage());
}

void ImageCropper::setCroppingFit()
{
    m_crop = false;
    m_fit = true;

    this->setCursor(Qt::ArrowCursor);

    update();
    emit imageCropped(this->getCroppedImage());
}

void ImageCropper::setCroppingStretch()
{
    m_crop = false;
    m_fit = false;

    this->setCursor(Qt::ArrowCursor);

    update();
    emit imageCropped(this->getCroppedImage());
}

void ImageCropper::setStretch(bool strech)
{
    m_stretch = strech;
    emit imageCropped(this->getCroppedImage());
}
