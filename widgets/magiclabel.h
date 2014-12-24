#ifndef MAGICLABEL_H
#define MAGICLABEL_H

#include <QtWidgets>
#include "../src/mousestate.h"

class MagicLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MagicLabel(QString text, QSpinBox*, QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

signals:

public slots:

private:
    QSpinBox *m_spinbox;
    int m_initValue;
    MouseState m_mouse;

};

#endif // MAGICLABEL_H
