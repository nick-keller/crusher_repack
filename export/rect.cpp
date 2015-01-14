#include "rect.h"

Rect::Rect(int x, int y, int width, int height) :
    m_rect(x, y, width, height)
{
}

QString Rect::getCode()
{
    return "Rect " + QString::number(m_rect.x()) + "," + QString::number(m_rect.y()) + "," + QString::number(m_rect.width()) + "," + QString::number(m_rect.height()) + "\n";
}

QString Rect::getFLineCode()
{
    QString code;
    QTextStream stream(&code);

    if(m_rect.width() < m_rect.height())
        for(int i(m_rect.x()); i < m_rect.x() + m_rect.width(); ++i)
            stream << "F-Line " << i +1 << "," << SCREEN_HEIGHT - m_rect.y() << "," << i +1 << "," << SCREEN_HEIGHT - m_rect.y() - m_rect.height() +1 << "<br>";
    else
        for(int i(m_rect.y()); i < m_rect.y() + m_rect.height(); ++i)
            stream << "F-Line " << m_rect.x() +1 << "," << SCREEN_HEIGHT - i << "," << m_rect.x() + m_rect.width() << "," << SCREEN_HEIGHT - i << "<br>";

    return code;
}

QString Rect::getForLoopCode(QString var)
{
    QString code;
    QTextStream stream(&code);

    if(m_rect.width() < m_rect.height()){
        stream << "For " << m_rect.x() +1 << "->" << var << " To " << m_rect.x() + m_rect.width() << "<br>";
        stream << "F-Line " << var << "," << SCREEN_HEIGHT - m_rect.y() << "," << var << "," << SCREEN_HEIGHT - m_rect.y() - m_rect.height() +1 << "<br>";
        stream << "Next<br>";
    }
    else{
        stream << "For " << SCREEN_HEIGHT - m_rect.y() - m_rect.height() +1 << "->" << var << " To " << SCREEN_HEIGHT - m_rect.y() << "<br>";
        stream << "F-Line " << m_rect.x() +1 << "," << var << "," << m_rect.x() + m_rect.width() << "," << var << "<br>";
        stream << "Next<br>";
    }

    return code;
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
    painter->fillRect(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.height(), Qt::black);
}
