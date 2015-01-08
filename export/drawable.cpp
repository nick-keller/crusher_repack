#include "drawable.h"

Drawable::Drawable()
{
}

QString Drawable::getCode()
{
    return QString();
}

int Drawable::weight() const
{
    return 0;
}


bool operator <(const Drawable &a, const Drawable &b)
{
    return a.weight() < b.weight();
}
