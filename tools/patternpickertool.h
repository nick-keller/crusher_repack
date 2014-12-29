#ifndef PATTERNPICKERTOOL_H
#define PATTERNPICKERTOOL_H

#include "tool.h"

class PatternPickerTool : public Tool
{
public:
    PatternPickerTool(ColorPicker *colorPicker);

    void mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);
    void mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);

    static QPixmap findPattern(QImage, QRect);

protected:
    void createToolBar();

private:
    QComboBox *m_saveIn;
};

#endif // PATTERNPICKERTOOL_H
