#ifndef DOT_H
#define DOT_H

#include "drawable.h"

class Dot : public Drawable
{
public:
    Dot(QPoint dot);

    QString getCode();
    QString getPlotOnCode();
    QString getPxlOnCode();
    QString getFLineCode();

    int weight() const;
    int x() {return m_point.x();}
    int y() {return m_point.y();}

    void paint(QPainter*);

private:
    QPoint m_point;
};

#endif // DOT_H
