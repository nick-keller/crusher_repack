#ifndef DOT_H
#define DOT_H

#include "drawable.h"

class Dot : public Drawable
{
public:
    Dot();

    int weight() const;

private:
    QPoint m_point;
};

#endif // DOT_H
