#include "layer.h"

Layer::Layer(int id) : m_visible(true), m_eyePixmap(":/icons/eye.png"), m_layer(SCREEN_WIDTH, SCREEN_HEIGHT, QImage::Format_ARGB32)
{
    m_name = "Layer " + QString::number(id);
    m_layer.fill(QColor(0, 0, 0, 0));
}

void Layer::paint(QPainter &painter, int width, bool selected)
{
    // Background
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(LAYER_BG_COLOR));
    painter.drawRect(0, 0, width, LAYER_HEIGHT);

    if(selected){
        painter.setBrush(QBrush(LAYER_SELECTED_COLOR));
        painter.drawRect(LAYER_EYE_WIDTH, 0, width - LAYER_EYE_WIDTH, LAYER_HEIGHT);

        painter.setPen(QPen(QColor(0, 0, 0)));
        painter.drawLine(LAYER_EYE_WIDTH +2, 8, LAYER_EYE_WIDTH +2, 1);
        painter.drawLine(LAYER_EYE_WIDTH +2, 1, LAYER_EYE_WIDTH +13, 1);

        painter.drawLine(LAYER_EYE_WIDTH +2, LAYER_HEIGHT -10, LAYER_EYE_WIDTH +2, LAYER_HEIGHT -3);
        painter.drawLine(LAYER_EYE_WIDTH +2, LAYER_HEIGHT -3, LAYER_EYE_WIDTH +13, LAYER_HEIGHT -3);

        painter.drawLine(LAYER_PREVIEW_WIDTH + LAYER_EYE_WIDTH -5, 1, LAYER_PREVIEW_WIDTH + LAYER_EYE_WIDTH +6, 1);
        painter.drawLine(LAYER_PREVIEW_WIDTH + LAYER_EYE_WIDTH +6, 1, LAYER_PREVIEW_WIDTH + LAYER_EYE_WIDTH +6, 8);

        painter.drawLine(LAYER_PREVIEW_WIDTH + LAYER_EYE_WIDTH +6, LAYER_HEIGHT -10, LAYER_PREVIEW_WIDTH + LAYER_EYE_WIDTH +6, LAYER_HEIGHT -3);
        painter.drawLine(LAYER_PREVIEW_WIDTH + LAYER_EYE_WIDTH +6, LAYER_HEIGHT -3, LAYER_PREVIEW_WIDTH + LAYER_EYE_WIDTH -5, LAYER_HEIGHT -3);
    }

    // Lines
    painter.setPen(QPen(LAYER_BORDER_COLOR));
    painter.drawLine(0, LAYER_HEIGHT -1, width, LAYER_HEIGHT -1);
    painter.drawLine(LAYER_EYE_WIDTH, 0, LAYER_EYE_WIDTH, LAYER_HEIGHT -1);

    // Name
    painter.setPen(QPen(QColor(0, 0, 0)));
    painter.drawText(QRect(LAYER_PREVIEW_WIDTH + LAYER_EYE_WIDTH +12, 0, 100, LAYER_HEIGHT-1), Qt::AlignLeft | Qt::AlignVCenter, m_name);

    // Preview
    QPixmap texture(8, 8);
    QPainter texturePainter(&texture);
    texture.fill(QColor(255, 255, 255));
    texturePainter.setPen(Qt::NoPen);
    texturePainter.setBrush(QBrush(QColor(204, 204, 204)));
    texturePainter.drawRect(0, 4, 4, 4);
    texturePainter.drawRect(4, 0, 4, 4);

    painter.setBrush(QBrush(texture));
    painter.setPen(QPen(LAYER_PREVIEW_BORDER_COLOR));
    QRect previewRect(LAYER_EYE_WIDTH +4, 3, LAYER_PREVIEW_WIDTH, LAYER_HEIGHT -8);
    painter.drawRect(previewRect);
    painter.drawImage(previewRect, m_layer);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(previewRect);

    // Eye
    painter.setPen(QPen(QColor(239, 239, 239)));
    painter.setBrush(Qt::NoBrush);
    if(m_visible)
        painter.drawPixmap(LAYER_EYE_WIDTH /2 -6, LAYER_HEIGHT /2 -6, m_eyePixmap);
    else
        painter.drawRect(LAYER_EYE_WIDTH /2 -6, LAYER_HEIGHT /2 -6, 11, 11);
}

void Layer::setVisible(bool visible)
{
    m_visible = visible;
}

void Layer::toggleVisible()
{
    m_visible = !m_visible;
}
