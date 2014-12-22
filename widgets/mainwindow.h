#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "workspace.h"
#include "colorpicker.h"
#include "../tools/tools.h"
#include "../dialogs/modifyselection.h"
#include "../dialogs/smoothselection.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    Tool *getTool();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void createMenus();
    void createToolBars();
    void createDocks();
    void createDialogs();
    Workspace *getWorkspace();

signals:

public slots:
    void createNewDocument();
    void closeDocument();
    void closeAllDocuments();
    void saveDocument();
    void saveDocumentAs();

    void cut();
    void copy();
    void paste();
    void clear();

    void createNewLayer();
    void removeLayer();

    void selectAll();
    void deselect();
    void reselect();
    void inverse();
    void expand();
    void contract();
    void smoothSelection();
    void selectionChanged(bool);

    void documentActivated(QMdiSubWindow * subWindow);
    void documentClosed();

    void toolSelected(QAction*);
    void cursorChanged(QCursor*);

private:
    QMdiArea                    *m_mdiArea;

    QHash<QString, QAction*>    m_actions;
    QHash<QString, QMenu*>      m_menus;
    QHash<QString, QDialog*>    m_dialogs;
    QMap<Tool::Name, Tool*>     m_tools;
    QList<QAction*>             m_actionsDocOpened;
    QList<QAction*>             m_actionsSelectionActivated;
    QList<QAction*>             m_actionsSelectionNotActivated;

    QScrollArea                 *m_layersArea;
    ColorPicker                 *m_colorPicker;
    QToolBar                    *m_toolOptions;

    Tool                        *m_tool;
    QImage                      m_clipboard;
};

#endif // MAINWINDOW_H
