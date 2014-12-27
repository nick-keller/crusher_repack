#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QtWidgets>
#include "../dialogs/brushselector.h"
#include "../src/const.h"

class ColorPicker : public QWidget
{
    Q_OBJECT
public:
    explicit ColorPicker(QWidget *parent = 0);

    BrushSelector::FillType getOutlineType(){return m_outlineSelector.getType();}
    BrushSelector::FillType getFillType(){return m_fillSelector.getType();}

    QPixmap getOutlinePattern(){return m_outlineSelector.getPattern();}
    QPixmap getFillPattern(){return m_fillSelector.getPattern();}

    QBrush getBrush(BrushSelector::FillType, QPixmap);
    QBrush getFillBrush();
    QBrush getOutlineBrush();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    QPixmap &getPixmap(BrushSelector::FillType type);

signals:

public slots:
    void swap();
    void init();
    void prompt(BrushSelector* selector);

private:
    BrushSelector m_fillSelector;
    BrushSelector m_outlineSelector;

    QPixmap m_background;
    QPixmap m_noFill;
    QPixmap m_blackFill;
    QPixmap m_whiteFill;
    QPixmap m_patternFill;

    QRect m_fillRect;
    QRect m_outlineRect;
    QRect m_swapRect;
    QRect m_initRect;

};

#endif // COLORPICKER_H
