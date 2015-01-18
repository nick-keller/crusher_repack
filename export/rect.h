#ifndef RECT_H
#define RECT_H

#include "drawable.h"

class Rect : public Drawable
{
public:
    Rect(int x, int y, int width, int height);

    QString getCode();
    QString getFLineCode();
    QString getForLoopCode(QString var);

    QList<QPoint> getDrawStatStops();

    int weight() const;

    int x() const {return m_rect.x();}
    int y() const {return m_rect.y();}
    int w() const {return m_rect.width();}
    int h() const {return m_rect.height();}

    void setX(int x);
    void setY(int y);
    void setRight(int r);
    void setBottom(int b);

    bool contains(int x, int y);

    void paint(QPainter*);

private:
    QRect m_rect;
};

#endif // RECT_H
