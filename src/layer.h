#ifndef LAYER_H
#define LAYER_H

#include <QtWidgets>

#define LAYER_HEIGHT 35
#define LAYER_EYE_WIDTH 30
#define LAYER_PREVIEW_WIDTH (LAYER_HEIGHT -8) *2 -1

#define LAYER_BG_COLOR QColor(214, 214, 214)
#define LAYER_BORDER_COLOR QColor(158, 158, 158)
#define LAYER_PREVIEW_BORDER_COLOR QColor(51, 51, 51)
#define LAYER_SELECTED_COLOR QColor(183, 205, 249)

class Layer
{
public:
    Layer(int id = 0);
    void paint(QPainter &painter, int width, bool selected);

    void setVisible(bool visible);
    bool getVisible(){return m_visible;}
    void toggleVisible();
    QImage *getImage(){return &m_layer;}

private:
    QString m_name;
    bool m_visible;
    QPixmap m_eyePixmap;
    QImage m_layer;
};

#endif // LAYER_H
