#include "mousestate.h"

MouseState::MouseState()
{
}

void MouseState::handleEvent(QMouseEvent *event, float scale, int offsetX, int offsetY)
{
    m_buttons = event->buttons();
    m_button = event->button();
    m_lastPos = m_pos;
    m_pos = this->scalePoint(event->pos(), scale, offsetX, offsetY);

    if(m_button == Qt::LeftButton && this->isButtonDown(Qt::LeftButton))
        m_clickedAt = m_pos;
}

bool MouseState::isButtonDown(Qt::MouseButton button)
{
    return m_buttons & button == button;
}

QRect MouseState::getClickedRect()
{
    QRect rect;

    rect.setX(std::min(m_pos.x(), m_clickedAt.x()));
    rect.setY(std::min(m_pos.y(), m_clickedAt.y()));
    rect.setWidth(std::abs(m_pos.x() - m_clickedAt.x()));
    rect.setHeight(std::abs(m_pos.y() - m_clickedAt.y()));

    return rect;
}

bool MouseState::moved()
{
    bool moved = m_lastMovedCheck != m_pos;
    m_lastMovedCheck = m_pos;
    return moved;
}

QPoint MouseState::scalePoint(QPoint point, float scale, int offsetX, int offsetY)
{
    QPoint scaled;
    scaled.setX(std::floor((point.x() - offsetX) / scale));
    scaled.setY(std::floor((point.y() - offsetY) / scale));

    return scaled;
}
