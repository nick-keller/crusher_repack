#ifndef MARQUEETOOL_H
#define MARQUEETOOL_H

#include "selectiontool.h"

class MarqueeTool : public SelectionTool
{
public:
    MarqueeTool(ColorPicker *colorPicker);

    void paintSelection(MouseState, QPainter*);
};

#endif // MARQUEETOOL_H
