#include "dot.h"

Dot::Dot(QPoint dot) : m_point(dot)
{
}

QString Dot::getCode()
{
    QString code;
    QTextStream(&code) << "Dot " << m_point.x() << "," << m_point.y();

    return code;
}

QString Dot::getPlotOnCode()
{
    QString code;
    QTextStream(&code) << "PlotOn " << m_point.x() +1 << "," << SCREEN_HEIGHT - m_point.y();

    return code;
}

QString Dot::getPxlOnCode()
{
    if(m_point.y() == 0 || m_point.x() == SCREEN_WIDTH -1)
        return "";

    QString code;
    QTextStream(&code) << "PxlOn " << m_point.y() << "," << m_point.x() +1;

    return code;
}

QString Dot::getFLineCode()
{
    QString code;
    QTextStream(&code) << "F-Line " << m_point.x() +1 << "," << SCREEN_HEIGHT - m_point.y() << "," << m_point.x() +1 << "," << SCREEN_HEIGHT - m_point.y();

    return code;
}

int Dot::weight() const
{
    return 1;
}

void Dot::paint(QPainter *painter)
{
    painter->drawPoint(m_point);
}
