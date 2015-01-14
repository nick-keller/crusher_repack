#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <QtWidgets>
#include <cmath>
#include "../src/const.h"

class Drawable
{
public:
    Drawable();

    virtual QString getCode();
    virtual int weight() const;
    virtual void paint(QPainter*){}
};

bool operator <(const Drawable& a, const Drawable& b);

#endif // DRAWABLE_H
