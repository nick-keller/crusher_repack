#include "lineiterator.h"

LineIterator::LineIterator(Line line) : LineIterator(line.from(), line.to())
{

}

LineIterator::LineIterator(QPoint from, QPoint to) :
    m_from(from), m_to(to), m_at(from), m_const(), m_hasNext(true)
{
    m_delta = to - from;

    if(std::abs(m_delta.x()) >= std::abs(m_delta.y())){
        m_const.setX(m_delta.x() > 0 ? 1 : -1);
        m_step = (std::abs(m_delta.x()) +1) / (std::abs(m_delta.y()) +1);
    }
    else{
        m_const.setY(m_delta.y() > 0 ? 1 : -1);
        m_step = (std::abs(m_delta.y()) +1) / (std::abs(m_delta.x()) +1);
    }
}

LineIterator &LineIterator::operator++()
{
    if(m_at == m_to)
        m_hasNext = false;

    m_at += m_const;

    if(m_const.y() == 0){
        if((m_at.x() - m_from.x()) % m_step == 0)
            m_at.setY(m_at.y() + (m_delta.y() > 0 ? 1 : -1));
    }
    else{
        if((m_at.y() - m_from.y()) % m_step == 0)
            m_at.setX(m_at.x() + (m_delta.x() > 0 ? 1 : -1));
    }

    return *this;
}

LineIterator LineIterator::operator++(int)
{
    LineIterator tmp(*this);
    operator++();
    return tmp;
}
