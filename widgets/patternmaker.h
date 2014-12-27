#ifndef PATTERNMAKER_H
#define PATTERNMAKER_H

#include <QtWidgets>
#include <cmath>
#include "../src/mousestate.h"
#include "../src/const.h"

#define MAX_WIDGET_SIZE 180
#define BACKGROUND_PATTERN_SIZE 8
#define DEFAULT_SIZE 2

class PatternMaker : public QWidget
{
    Q_OBJECT
public:
    enum Color {Black, White, Transparent};
    explicit PatternMaker(QWidget *parent = 0);

    QPixmap getPattern(){return m_pattern;}
    void setPattern(QPixmap pixmap);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void updateSize();
    void drawPoint();

    int getOffsetX();
    int getOffsetY();

signals:

public slots:
    void setWidth(int);
    void setHeight(int);
    void setColor(int);

private:
    int m_width;
    int m_height;
    QPixmap m_pattern;
    QBrush m_backgroundBrush;
    Color m_color;
    int m_pixelWidth;
    MouseState m_mouse;

};

#endif // PATTERNMAKER_H
