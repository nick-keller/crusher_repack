#ifndef TOOL_H
#define TOOL_H

#include <QtWidgets>
#include <cmath>
#include "../src/mousestate.h"
#include "../widgets/colorpicker.h"

class Tool : public QObject
{
    Q_OBJECT
    
public:
    enum Name {Move, Marquee, EllipseSelection, Rect, Ellipse, Line};
    enum Cursor {Cross, CrossPlus, CrossMinus, CrossAnd, Duplicate, Hand, MoveCursor, MoveCut, Selection, SelectLayer};

    Tool(ColorPicker* colorPicker, Name toolName, QString name, QString icon, QString shortcut = "");

    QAction         *getAction(QObject *parent);
    QToolBar        *getToolBar(QWidget *parent);
    QCursor         *getCursor();

    // Events to override
    virtual void mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection){}
    virtual void mousePressEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection){}
    virtual void mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection){}

protected:
    virtual void createToolBar(){}

    // Widgets
    QComboBox   *createAngleSelector();
    QComboBox   *createOutlineSelector();
    QComboBox   *createCapSelector();
    QSpinBox    *createBrushSizeSelector();

    // Daw helper
    void drawPerfectLine(QPainter *painter, int x1, int y1, int x2, int y2);
    void drawPerfectLine(QPainter *painter, QPoint p1, QPoint p2);
    void drawPerfectEllipse(QPainter *painter, int x, int y, int width, int height, bool fill = false);
    void drawPerfectEllipse(QPainter *painter, QRect ellipse, bool fill = false);
    void drawInSelection(QImage *target, QImage *source, QImage *selection, bool *useSelection);

    // Painter helpers
    QPainter    *getPainter(QPaintDevice *device);
    QBrush      getBrush(BrushSelector::FillType, QPixmap);
    QBrush      getFillBrush();
    QPen        getFillPen();
    QBrush      getOutlineBrush();
    QPen        getOutlinePen();

    // Helpers
    QRect getRectPlusOne(QRect);
    QRect getRectPlusOne(MouseState);

    // Other
    void setCursor(Cursor);

signals:
    void selectionChanged(bool);
    void cursorChanged(QCursor*);

protected:
    QString                 m_name;
    QString                 m_shortcut;
    QString                 m_icon;
    QToolBar                *m_toolbar;
    Name                    m_toolName;
    ColorPicker             *m_colorPicker;
    QSpinBox                *m_brushSize;
    QComboBox               *m_joinType;
    QComboBox               *m_outlineType;
    QComboBox               *m_capType;
    QCursor                 *m_cursor;
    QHash<Cursor, QCursor*> m_cursors;
};

#endif // TOOL_H