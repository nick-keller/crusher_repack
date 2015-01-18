#ifndef LINE_H
#define LINE_H

#include "drawable.h"
#include "../tools/tool.h"

class Line : public Drawable
{
public:
    Line(int x1, int y1, int x2, int y2);

    QString getCode();
    QString getFLineCode();

    int weight() const;
    QPoint from() {return m_p1;}
    QPoint to() {return m_p2;}

    void setWeight(int w);

    void paint(QPainter*);

    void constrain(int minX, int minY, int maxX, int maxY);

    bool isDot();

private:
    QPoint m_p1;
    QPoint m_p2;

    int m_weight;
};

#endif // LINE_H
