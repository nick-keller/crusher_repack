#ifndef ELLIPSESELECTIONTOOL_H
#define ELLIPSESELECTIONTOOL_H

#include "selectiontool.h"

class EllipseSelectionTool : public SelectionTool
{
public:
    EllipseSelectionTool(ColorPicker *colorPicker);

    void paintSelection(MouseState, QPainter*);
};

#endif // ELLIPSESELECTIONTOOL_H
