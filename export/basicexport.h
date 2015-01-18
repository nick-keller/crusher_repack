#ifndef BASICEXPORT_H
#define BASICEXPORT_H

#include <QtWidgets>
#include "../src/const.h"
#include "../dialogs/basicexportdialog.h"
#include "../dialogs/codedisplaydialog.h"
#include "drawable.h"
#include "line.h"
#include "rect.h"
#include "dot.h"
#include "lineiterator.h"

typedef QVector< QVector<int> > IntMap;
typedef QVector< QVector<bool> > BoolMap;

class BasicExport
{
public:
    BasicExport(QImage image);
    void generate();

private:
    void convertImage(QImage);
    void findDots();
    void findLines();
    void findRects();

    bool isPxlLeftToDraw(int x, int y);
    bool isPxlOn(int x, int y);
    bool isHLineLeftToDraw(int x, int y, int length);
    bool isHLineOn(int x, int y, int length);
    bool isVLineLeftToDraw(int x, int y, int length);
    bool isVLineOn(int x, int y, int length);
    bool lineHasAtLeastOnePxlLeftToDraw(int x1, int y1, int x2, int y2);

    void addLineToRedundancyMap(IntMap& map, Line line, int value = 1);
    bool lineHasExclusivePixel(IntMap& map, Line line);

    void addRectToRedundancyMap(IntMap& map, Rect r, int value = 1);
    bool rectHasExclusivePixel(IntMap& map, Rect rect);

    QString getRectsCode(BasicExportDialog*);
    QString getLinesCode(BasicExportDialog*);
    QString getDotsCode(BasicExportDialog*);

    IntMap getIntMap();
    BoolMap getBoolMap();

private:
    int m_width;
    int m_height;
    BoolMap m_originalImage;
    BoolMap m_leftToDraw;
    BoolMap m_rectArea;
    BoolMap m_lineArea;
    BoolMap m_dotArea;
    QList<Rect> m_rects;
    QList<Line> m_lines;
    QList<Dot> m_dots;
};

#endif // BASICEXPORT_H
