#include "line.h"
#include "lineiterator.h"

Line::Line(int x1, int y1, int x2, int y2) :
    m_p1(x1, y1), m_p2(x2, y2), m_weight(-1)
{
}

QString Line::getCode()
{
    return "F-Line " + QString::number(m_p1.x()) + "," + QString::number(m_p1.y()) + "," + QString::number(m_p2.x()) + "," + QString::number(m_p2.y()) + "\n";
}

int Line::weight() const
{
    if(m_weight != -1)
        return m_weight;

    QPoint vec = m_p1 - m_p2;
    return std::max(std::abs(vec.x()), std::abs(vec.y())) +1;
}

void Line::setWeight(int w)
{
    m_weight = w;
}

void Line::paint(QPainter *painter)
{
    for(LineIterator point(*this); point.hasNext(); ++point)
        painter->drawPoint(point.x(), point.y());
}

void Line::constrain(int minX, int minY, int maxX, int maxY)
{
    QPoint delta = m_p2 - m_p1;
    QPoint constAdd(0,0);
    QPoint at = m_p1;
    QPoint newFrom(-1, -1);
    QPoint newTo(-1, -1);

    int step;

    if(std::abs(delta.x()) >= std::abs(delta.y())){
        constAdd.setX(delta.x() > 0 ? 1 : -1);
        step = (std::abs(delta.x()) +1) / (std::abs(delta.y()) +1);
    }
    else{
        constAdd.setY(delta.y() > 0 ? 1 : -1);
        step = (std::abs(delta.y()) +1) / (std::abs(delta.x()) +1);
    }

    QPoint end = m_p2 + constAdd;
    if(constAdd.y() == 0){
        end.setY(end.y() + (delta.y() > 0 ? 1 : -1));
    }
    else{
       end.setX(end.x() + (delta.x() > 0 ? 1 : -1));
    }

    while(at != end){

        for(int i(0); i < step; ++i){
            QPoint temp = at + i*constAdd;

            if(temp.x() >= minX && temp.y() >= minY && temp.x() <= maxX && temp.y() <= maxY){
                if(newFrom == QPoint(-1, -1))
                    newFrom = at;
                newTo = at + (step-1) * constAdd;
            }
        }

        at += step * constAdd;

        if(constAdd.y() == 0){
            at.setY(at.y() + (delta.y() > 0 ? 1 : -1));
        }
        else{
           at.setX(at.x() + (delta.x() > 0 ? 1 : -1));
        }
    }

    m_p1 = newFrom;
    m_p2 = newTo;

    if(m_p1.y() == m_p2.y()){
        m_p1.setX(std::max(minX, std::min(maxX, m_p1.x())));
        m_p2.setX(std::max(minX, std::min(maxX, m_p2.x())));
    }

    if(m_p1.x() == m_p2.x()){
        m_p1.setY(std::max(minY, std::min(maxY, m_p1.y())));
        m_p2.setY(std::max(minY, std::min(maxY, m_p2.y())));
    }
}

bool Line::isDot()
{
    return m_p1 == m_p2;
}
