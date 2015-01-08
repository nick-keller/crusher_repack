#ifndef LINE_H
#define LINE_H

#include "drawable.h"

class Line : public Drawable
{
public:
    Line(int x1, int y1, int x2, int y2);

    QString getCode();
    int weight() const;

private:
    QPoint m_p1;
    QPoint m_p2;
};

#endif // LINE_H
