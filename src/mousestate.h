#ifndef MOUSESTATE_H
#define MOUSESTATE_H

#include <QtWidgets>
#include <cmath>

class MouseState
{
public:
    MouseState();
    void handleEvent(QMouseEvent *event, float scale = 1, int offsetX = 0, int offsetY = 0);
    bool isButtonDown(Qt::MouseButton button);
    Qt::MouseButton button(){return m_button;}

    QPoint getPos(){return m_pos;}
    QPoint getLastPos(){return m_lastPos;}
    QPoint getClickedAt(){return m_clickedAt;}
    QPoint delta();
    QRect getClickedRect();

    bool moved();

    int x(){return m_pos.x();}
    int y(){return m_pos.y();}
    int clickedX(){return m_clickedAt.x();}
    int clickedY(){return m_clickedAt.y();}

private:
    QPoint scalePoint(QPoint point, float scale, int offsetX, int offsetY);

private:
    QPoint m_pos;
    QPoint m_lastPos;
    QPoint m_clickedAt;
    QPoint m_lastMovedCheck;
    Qt::MouseButtons m_buttons;
    Qt::MouseButton m_button;
};

#endif // MOUSESTATE_H
