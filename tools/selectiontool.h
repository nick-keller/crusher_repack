#ifndef SELECTIONTOOL_H
#define SELECTIONTOOL_H

#include "tool.h"

#define GREEN_SELECTION QColor(137, 187, 78)

class SelectionTool : public Tool
{
    Q_OBJECT

public:
    SelectionTool(ColorPicker* colorPicker, Name toolName, QString name, QString icon, QString shortcut = "");

protected:
    void mousePressEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);
    void mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);
    void mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);

    void createToolBar();
    void *createSelectionModeSelector();
    void setCorrectCursor();

    virtual void paintSelection(MouseState, QPainter*){}

public slots:
    void changeSelectionMode(QAction* action);

protected:
    QString m_selectionMode;
    QImage m_movingSelection;
    bool m_moving;
};

#endif // SELECTIONTOOL_H
