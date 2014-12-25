#include "imagecropper.h"

ImageCropper::ImageCropper(QWidget *parent) :
    QWidget(parent)
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
}

void ImageCropper::paintEvent(QPaintEvent *event)
{

    // Background
    QPainter painter(this);
    painter.fillRect(0, 0, SIZE, SIZE, QColor(147, 147, 147));

    // Image
    painter.drawImage(m_imageRect, m_image);

    // Cropping zone
    painter.fillRect(0, 0, SIZE, m_croppingZone.y(), QColor(0, 0, 0, 128));
    painter.fillRect(0, m_croppingZone.y(), m_croppingZone.x(), m_croppingZone.height(), QColor(0, 0, 0, 128));
    painter.fillRect(m_croppingZone.right(), m_croppingZone.y(), SIZE, m_croppingZone.height(), QColor(0, 0, 0, 128));
    painter.fillRect(0, m_croppingZone.bottom(), SIZE, SIZE, QColor(0, 0, 0, 128));

    painter.setBrush(QBrush(Qt::NoBrush));
    painter.setPen(QPen(Qt::red));
    painter.drawRect(m_croppingZone.x(), m_croppingZone.y(), m_croppingZone.width() -1, m_croppingZone.height() -1);

    // Border
    painter.setPen(QPen(QColor(40, 40, 40)));
    painter.setBrush(QBrush(Qt::NoBrush));
    painter.drawRect(0, 0, this->width() -1, this->height() -1);
}
