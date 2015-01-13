#include "dot.h"

Dot::Dot(QPoint dot) : m_point(dot)
{
}

QString Dot::getCode()
{
    return "Plot-On " + QString::number(m_point.x()) + "," + QString::number(m_point.y()) + "\n";
}

int Dot::weight() const
{
    return 1;
}

void Dot::paint(QPainter *painter)
{
    painter->drawPoint(m_point);
}
