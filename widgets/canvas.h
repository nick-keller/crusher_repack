#ifndef CANVAS_H
#define CANVAS_H

#define BORDER QColor(128, 128, 128, 100)
#define BACKGROUND QColor(204,204,204)

#define GRID_MIN_ZOOM_LEVEL 6
#define INIT_ZOOM_LEVEL 6

#include <QtWidgets>
#include "layerstack.h"
#include "../src/const.h"

class MainWindow;
class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(MainWindow *mainWindow, QWidget *parent = 0);

    void zoom(float level);
    float getZoom() {return m_zoomLevel;}
    LayerStack *getLayerStack(){return m_layers;}

    bool isSelectionActivated(){return m_selectionActivated;}
    void toggleSelection();
    void selectAll();
    void inverseSelection();
    void expandSelection(int radius, bool sharp);
    void contractSelection(int radius, bool sharp);

    QImage cut();
    QImage copy();
    void paste(QImage);
    void clear();
    void fillSelection(QBrush);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    void initPaintDevice();
    bool isPointSelected(int x, int y);
    void modifySelection(int radius, bool sharp, QColor expand);

signals:

public slots:

private:
    QPen                        m_penGrid;
    QBrush                      m_brushBackground;
    QPen                        m_drawingPen;
    QBrush                      m_drawingBrush;
    QPen                        m_selectionPen;
    float                       m_zoomLevel;
    LayerStack                  *m_layers;
    MainWindow                  *m_mainWindow;
    MouseState                  m_mouse;

    QImage                      m_selection;
    bool                        m_selectionActivated;
    QBasicTimer                 m_selectionTimer;

};

#endif // CANVAS_H
