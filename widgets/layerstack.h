#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include <QtWidgets>
#include <cmath>
#include "../src/layer.h"
#include "../src/mousestate.h"

class LayerStack : public QWidget
{
    Q_OBJECT
public:
    explicit LayerStack(QWidget *parent = 0);
    int count(){return m_layers.count();}
    int isLayerVisible(int i);

    QImage *getImage(int i){return m_layers[i].getImage();}
    QImage *getImage(){return m_layers[m_selectedLayer].getImage();}
    QImage *getHud(){return m_hud.getImage();}

    QImage getFlattenedImage();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void updateHeight();

    int getMidLayerIndex(int y);
    int getMidLayerIndex();
    int getLayerIndex(int y);
    int getLayerIndex();

    bool isInEyeArea(int x);
    bool isInEyeArea();
    bool clickedInEyeArea();

    bool isMouseDown();

signals:
    void modified();

public slots:
    void newLayer();
    void removeLayer();

private:
    QList<Layer> m_layers;
    Layer m_hud;
    int m_selectedLayer;
    MouseState m_mouse;
    bool m_movingLayers;

};

#endif // LAYERSTACK_H
