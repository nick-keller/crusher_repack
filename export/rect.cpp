#include "rect.h"

Rect::Rect(int x, int y, int width, int height) :
    m_rect(x, y, width, height)
{
}

QString Rect::getCode()
{
    return "Rect " + QString::number(m_rect.x()) + "," + QString::number(m_rect.y()) + "," + QString::number(m_rect.width()) + "," + QString::number(m_rect.height()) + "\n";
}

int Rect::weight() const
{
    return m_rect.size().height() * m_rect.size().width();
}

void Rect::setX(int x)
{
    m_rect.setX(x);
}

void Rect::setY(int y)
{
    m_rect.setY(y);
}

void Rect::setRight(int r)
{
    m_rect.setWidth(r - m_rect.x() +1);
}

void Rect::setBottom(int b)
{
    m_rect.setHeight((b - m_rect.y() +1));
}

bool Rect::contains(int x, int y)
{
    if(x < m_rect.x())
        return false;
    if(y < m_rect.y())
        return false;
    if(x >= m_rect.x() + m_rect.width())
        return false;
    if(y >= m_rect.y() + m_rect.height())
        return false;
    return true;
}

void Rect::paint(QPainter *painter)
{
    painter->fillRect(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.height(), QColor(qrand() %255, qrand() %255, qrand() %255, 128));
}
