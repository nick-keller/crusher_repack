#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <QtWidgets>
#include <cmath>

class Drawable
{
public:
    Drawable();

    virtual QString getCode();
    virtual int weight() const;
};

bool operator <(const Drawable& a, const Drawable& b);

#endif // DRAWABLE_H