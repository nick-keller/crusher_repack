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
