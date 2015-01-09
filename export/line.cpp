#include "line.h"

Line::Line(int x1, int y1, int x2, int y2) :
    m_p1(x1, y1), m_p2(x2, y2)
{
}

QString Line::getCode()
{
    return "F-Line " + QString::number(m_p1.x()) + "," + QString::number(m_p1.y()) + "," + QString::number(m_p2.x()) + "," + QString::number(m_p2.y()) + "\n";
}

int Line::weight() const
{
    QPoint vec = m_p1 - m_p2;
    return std::max(std::abs(vec.x()), std::abs(vec.y())) +1;
}
