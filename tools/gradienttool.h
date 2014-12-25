#ifndef GRADIENTTOOL_H
#define GRADIENTTOOL_H

#include "tool.h"

class GradientTool : public Tool
{
public:
    GradientTool(ColorPicker *colorPicker);

    void mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);
    void mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);

protected:
    void createToolBar();

private:
    void drawGradient(MouseState mouse, QImage* image, QColor from, QColor to, bool dithering);
    QColor getColor(QString);

private:
    QComboBox *m_from;
    QComboBox *m_to;
    QCheckBox *m_dithering;
    QSpinBox *m_ditheringLevel;
};

#endif // GRADIENTTOOL_H
