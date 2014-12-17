#include "patternmaker.h"

PatternMaker::PatternMaker(QWidget *parent) :
    QWidget(parent), m_width(DEFAULT_SIZE), m_height(DEFAULT_SIZE), m_color(Black), m_pattern(0,0), m_pixelWidth(1)
{
    QPixmap backgroundTexture(BACKGROUND_PATTERN_SIZE *2, BACKGROUND_PATTERN_SIZE *2);
    backgroundTexture.fill(QColor(255, 255, 255));
    QPainter bgPainter(&backgroundTexture);
    bgPainter.setPen(Qt::NoPen);
    bgPainter.setBrush(QBrush(QColor(204, 204, 204)));
    bgPainter.drawRect(0, BACKGROUND_PATTERN_SIZE, BACKGROUND_PATTERN_SIZE, BACKGROUND_PATTERN_SIZE);
    bgPainter.drawRect(BACKGROUND_PATTERN_SIZE, 0, BACKGROUND_PATTERN_SIZE, BACKGROUND_PATTERN_SIZE);

    m_backgroundBrush.setTexture(backgroundTexture);

    this->updateSize();
    this->setFixedSize(MAX_WIDGET_SIZE +1, MAX_WIDGET_SIZE +1);
    this->setCursor(Qt::PointingHandCursor);
}

void PatternMaker::setPattern(QPixmap pixmap)
{
    this->setWidth(pixmap.width());
    this->setHeight(pixmap.height());
    m_pattern = pixmap;
}

void PatternMaker::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if(!this->isEnabled())
        painter.setOpacity(.5);

    // Center view
    painter.translate(this->getOffsetX(), this->getOffsetY());

    // Background
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_backgroundBrush);
    painter.drawRect(0, 0, m_width * m_pixelWidth, m_height * m_pixelWidth);

    // Pattern
    painter.drawPixmap(0, 0, m_width * m_pixelWidth, m_height * m_pixelWidth, m_pattern);

    // Grid
    painter.setPen(QPen(QColor(128, 128, 128)));
    for(int i(0); i <= m_width; ++i)
        painter.drawLine(i * m_pixelWidth, 0, i * m_pixelWidth, m_height * m_pixelWidth);
    for(int i(0); i <= m_height; ++i)
        painter.drawLine(0, i * m_pixelWidth, m_width * m_pixelWidth, i * m_pixelWidth);
}

void PatternMaker::mouseMoveEvent(QMouseEvent *event)
{
    m_mouse.handleEvent(event, m_pixelWidth, this->getOffsetX(), this->getOffsetY());
    this->drawPoint();
    update();
}

void PatternMaker::mousePressEvent(QMouseEvent *event)
{
    m_mouse.handleEvent(event, m_pixelWidth, this->getOffsetX(), this->getOffsetY());
    this->drawPoint();
    update();
}

void PatternMaker::mouseReleaseEvent(QMouseEvent *event)
{
    m_mouse.handleEvent(event, m_pixelWidth, this->getOffsetX(), this->getOffsetY());
}

void PatternMaker::updateSize()
{
    m_pixelWidth = std::min(MAX_WIDGET_SIZE / m_width, MAX_WIDGET_SIZE / m_height);

    QPixmap pattern(m_width, m_height);
    pattern.fill(QColor(0, 0, 0, 0));

    // Copy old pattern on new one
    QPainter pTemp(&pattern);
    pTemp.drawPixmap(0, 0, m_pattern);

    m_pattern = pattern;

    update();
}

void PatternMaker::drawPoint()
{
    QPainter painter(&m_pattern);

    if(m_color == Black || m_color == White){
        if(m_color == Black)
            painter.setPen(QColor(0, 0, 0));
        else
            painter.setPen(QColor(255, 255, 255));

        painter.drawPoint(m_mouse.x(), m_mouse.y());
    }
    else{
        painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
        painter.drawPoint(m_mouse.x(), m_mouse.y());
    }
}

int PatternMaker::getOffsetX()
{
    return (this->width() - m_width * m_pixelWidth) / 2;
}

int PatternMaker::getOffsetY()
{
    return (this->height() - m_height * m_pixelWidth) / 2;
}

void PatternMaker::setWidth(int width)
{
    m_width = width;
    this->updateSize();
}

void PatternMaker::setHeight(int height)
{
    m_height = height;
    this->updateSize();
}

void PatternMaker::setColor(int color)
{
    m_color = static_cast<Color>(color);
}
