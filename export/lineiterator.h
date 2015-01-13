#ifndef LINEITERATOR_H
#define LINEITERATOR_H

#include <cmath>
#include "line.h"

class LineIterator
{
public:
    LineIterator(Line);
    LineIterator(QPoint from, QPoint to);

    QPoint at() {return m_at;}
    bool hasNext() {return m_hasNext;}
    int x() {return m_at.x();}
    int y() {return m_at.y();}

    LineIterator& operator++();
    LineIterator operator++(int);


private:
    QPoint m_from;
    QPoint m_to;
    QPoint m_at;
    QPoint m_delta;

    QPoint m_const;
    bool m_hasNext;
    int m_step;
};

#endif // LINEITERATOR_H
