#include "magiclabel.h"

MagicLabel::MagicLabel(QString text, QSpinBox *spinBox, QWidget *parent) :
    QLabel(text, parent),
    m_spinbox(spinBox)
{
    this->setCursor(QCursor(QPixmap(":/cursors/setting.png"), 7, 5));
}

void MagicLabel::mousePressEvent(QMouseEvent *event)
{
    m_mouse.handleEvent(event);

    if(m_mouse.button() == Qt::LeftButton)
        m_initValue = m_spinbox->value();
}

void MagicLabel::mouseMoveEvent(QMouseEvent *event)
{
    m_mouse.handleEvent(event);

    if(m_mouse.isButtonDown(Qt::LeftButton)){
        m_spinbox->setValue(m_mouse.delta().x() /3 + m_initValue);
    }
}

void MagicLabel::mouseReleaseEvent(QMouseEvent *event)
{
    m_mouse.handleEvent(event);
}
