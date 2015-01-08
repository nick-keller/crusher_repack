#ifndef RECT_H
#define RECT_H

#include "drawable.h"

class Rect : public Drawable
{
public:
    Rect();

    int weight() const;

private:
    QRect m_rect;
};

#endif // RECT_H
