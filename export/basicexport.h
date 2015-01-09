#ifndef BASICEXPORT_H
#define BASICEXPORT_H

#include <QtWidgets>
#include "../src/const.h"
#include "../dialogs/basicexportdialog.h"
#include "../dialogs/codedisplaydialog.h"
#include "drawable.h"
#include "line.h"
#include "rect.h"

typedef QVector< QVector<int> > IntMap;

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

    bool isPxlOn(int x, int y);
    bool isHLineOn(int x, int y, int length);
    bool isVLineOn(int x, int y, int length);

    IntMap getIntMap();

private:
    int m_width;
    int m_height;
    QVector< QVector<bool> > m_image;
    QVector<Drawable*> m_drawables;
};

#endif // BASICEXPORT_H
