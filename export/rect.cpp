#include "rect.h"

Rect::Rect()
{
}

int Rect::weight() const
{
    return m_rect.size().height() * m_rect.size().width();
}
