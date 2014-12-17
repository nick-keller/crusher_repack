#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QtWidgets>
#include "canvas.h"

class Workspace : public QScrollArea
{
    Q_OBJECT
public:
    explicit Workspace(MainWindow *, QWidget *parent = 0);
    void setTitle(const QString);
    void setModified(bool);
    LayerStack* getLayerStack() {return m_canvas->getLayerStack();}
    Canvas *getCanvas(){return m_canvas;}

protected:
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    void updateWindowTitle();
    void updateCursor();
    Qt::Key getKey(QKeyEvent *event);

signals:

public slots:
    bool save();
    bool saveAs();

private:
    Canvas          *m_canvas;
    QSet<Qt::Key>   m_keysDown;
    QString         m_title;
    bool            m_modified;
    MouseState      m_mouse;
    QString         m_file;
    MainWindow      *m_mainWindow;
};

#endif // WORKSPACE_H
