#ifndef DOT_H
#define DOT_H

#include "drawable.h"

class Dot : public Drawable
{
public:
    Dot(QPoint dot);

    QString getCode();
    int weight() const;
    void paint(QPainter*);

private:
    QPoint m_point;
};

#endif // DOT_H
