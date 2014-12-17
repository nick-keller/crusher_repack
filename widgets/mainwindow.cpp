#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_toolOptions(0)
{
    m_mdiArea = new QMdiArea();
    m_mdiArea->setViewMode(QMdiArea::TabbedView);
    m_mdiArea->setTabsClosable(true);
    m_mdiArea->setTabsMovable(true);

    this->setWindowTitle("Crusher");
    this->setCentralWidget(m_mdiArea);
    this->createMenus();
    this->createToolBars();
    this->createDocks();
    this->setWindowIcon(QIcon(":/icons/logo.png"));

    QObject::connect(m_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(documentActivated(QMdiSubWindow*)));
}

Tool *MainWindow::getTool()
{
    return m_tool;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    this->closeAllDocuments();

    if(m_mdiArea->subWindowList().count())
        event->ignore();
}

void MainWindow::createMenus()
{
    QMenuBar *menuBar = this->menuBar();

    // File ------------------------------------------------------------------------------
    QMenu *menuFile = menuBar->addMenu("&File");

    m_actions.insert("new", menuFile->addAction("New", this, SLOT(createNewDocument()), QString("Ctrl+N")));
    menuFile->addSeparator();
    m_actions.insert("close", menuFile->addAction("Close", this, SLOT(closeDocument()), QString("Ctrl+W")));
    m_actions.insert("close all", menuFile->addAction("Close all", this, SLOT(closeAllDocuments()), QString("Alt+Ctrl+W")));
    m_actions.insert("save", menuFile->addAction("Save", this, SLOT(saveDocument()), QString("Ctrl+S")));
    m_actions.insert("save as", menuFile->addAction("Save as...", this, SLOT(saveDocumentAs()), QString("Ctrl+Shift+S")));
    menuFile->addSeparator();
    m_actions.insert("exit", menuFile->addAction("Exit", this, SLOT(close()), QString("Ctrl+Q")));

    m_actions["close"]->setDisabled(true);
    m_actions["close all"]->setDisabled(true);
    m_actions["save"]->setDisabled(true);
    m_actions["save as"]->setDisabled(true);

    // Edit ------------------------------------------------------------------------------
    QMenu *menuEdit = menuBar->addMenu("&Edit");

    m_actions.insert("cut", menuEdit->addAction("Cut", this, SLOT(cut()), QString("Ctrl+X")));
    m_actions.insert("copy", menuEdit->addAction("Copy", this, SLOT(copy()), QString("Ctrl+C")));
    m_actions.insert("paste", menuEdit->addAction("Paste", this, SLOT(paste()), QString("Ctrl+V")));
    m_actions.insert("clear", menuEdit->addAction("Clear", this, SLOT(clear()), QString("Del")));

    m_actions["cut"]->setDisabled(true);
    m_actions["copy"]->setDisabled(true);
    m_actions["paste"]->setDisabled(true);
    m_actions["clear"]->setDisabled(true);

    // Layer -----------------------------------------------------------------------------
    QMenu *menuLayer = menuBar->addMenu("&Layer");

    m_actions.insert("new layer", menuLayer->addAction(QIcon(":/icons/newlayer.png"), "New layer...", this, SLOT(createNewLayer()),  QString("Shift+Ctrl+N")));
    m_actions.insert("remove layer", menuLayer->addAction(QIcon(":/icons/trash.png"), "Remove layer", this, SLOT(removeLayer())));

    m_actions["new layer"]->setDisabled(true);
    m_actions["remove layer"]->setDisabled(true);

    // Select ----------------------------------------------------------------------------
    QMenu *menuSelect = menuBar->addMenu("&Select");

    m_actions.insert("select all", menuSelect->addAction("All", this, SLOT(selectAll()), QString("Ctrl+A")));
    m_actions.insert("deselect", menuSelect->addAction("Deselect", this, SLOT(deselect()), QString("Ctrl+D")));
    m_actions.insert("reselect", menuSelect->addAction("Reselect", this, SLOT(reselect()), QString("Ctrl+Shift+D")));
    m_actions.insert("inverse", menuSelect->addAction("Inverse", this, SLOT(inverse()), QString("Ctrl+Shift+I")));
    menuSelect->addSeparator();

    m_menus.insert("modify", menuSelect->addMenu("Modify"));

    m_actions.insert("expand", m_menus["modify"]->addAction("Expand...", this, SLOT(expand())));
    m_actions.insert("contract", m_menus["modify"]->addAction("Contract...", this, SLOT(expand())));

    m_actions["select all"]->setDisabled(true);
    m_actions["deselect"]->setDisabled(true);
    m_actions["reselect"]->setDisabled(true);
    m_actions["inverse"]->setDisabled(true);
    m_menus["modify"]->setEnabled(false);
}

void MainWindow::createToolBars()
{
    QToolBar *tools = new QToolBar("Tools");
    tools->setAllowedAreas(Qt::LeftToolBarArea);
    tools->setFloatable(false);
    tools->setMovable(false);

    // create colorpicker
    m_colorPicker = new ColorPicker();

    // create tools
    m_tools.insert(Tool::Move, new MoveTool(m_colorPicker));
    m_tools.insert(Tool::Marquee, new MarqueeTool(m_colorPicker));
    m_tools.insert(Tool::EllipseSelection, new EllipseSelectionTool(m_colorPicker));
    m_tools.insert(Tool::Rect, new RectTool(m_colorPicker));
    m_tools.insert(Tool::Ellipse, new EllipseTool(m_colorPicker));
    m_tools.insert(Tool::Line, new LineTool(m_colorPicker));

    // create toolbar
    QActionGroup *toolGroup = new QActionGroup(this);
    QMapIterator<Tool::Name, Tool*> i(m_tools);
    while(i.hasNext()){
        i.next();
        QAction *action = i.value()->getAction(this);
        toolGroup->addAction(action);
        tools->addAction(action);

        QObject::connect(i.value(), SIGNAL(selectionChanged(bool)), this, SLOT(selectionChanged(bool)));
        QObject::connect(i.value(), SIGNAL(cursorChanged(QCursor*)), this, SLOT(cursorChanged(QCursor*)));
    }
    tools->addSeparator();
    QObject::connect(toolGroup, SIGNAL(triggered(QAction*)), this, SLOT(toolSelected(QAction*)));
    toolGroup->actions().first()->trigger();

    // add colorpicker
    tools->addWidget(m_colorPicker);

    this->addToolBar(Qt::LeftToolBarArea, tools);
}

void MainWindow::createDocks()
{
    m_layersArea = new QScrollArea;
    m_layersArea->setBackgroundRole(QPalette::Dark);
    m_layersArea->setWidgetResizable(true);
    m_layersArea->setMinimumWidth(250);

    QPushButton *newLayerButton = new QPushButton(QIcon(":/icons/newlayer.png"), "");
    QPushButton *removeLayerButton = new QPushButton(QIcon(":/icons/trash.png"), "");
    QObject::connect(newLayerButton, SIGNAL(clicked()), m_actions["new layer"], SLOT(trigger()));
    QObject::connect(removeLayerButton, SIGNAL(clicked()), m_actions["remove layer"], SLOT(trigger()));

    QHBoxLayout *toolBarLayout = new QHBoxLayout;
    toolBarLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    toolBarLayout->addWidget(newLayerButton);
    toolBarLayout->addWidget(removeLayerButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_layersArea);
    mainLayout->addLayout(toolBarLayout);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *dockWidget = new QWidget;
    dockWidget->setLayout(mainLayout);

    QDockWidget *layersDock = new QDockWidget("Layers");
    layersDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    layersDock->setFeatures(QDockWidget::DockWidgetMovable);
    layersDock->setWidget(dockWidget);

    this->addDockWidget(Qt::RightDockWidgetArea, layersDock);
}

Workspace *MainWindow::getWorkspace()
{
    if(m_mdiArea->currentSubWindow() == 0)
        return 0;

    return (Workspace*) m_mdiArea->currentSubWindow()->widget();
}

void MainWindow::createNewDocument()
{
    Workspace *newDocument = new Workspace(this);
    QObject::connect(newDocument, SIGNAL(destroyed()), this, SLOT(documentClosed()));

    QMdiSubWindow *subWindow = m_mdiArea->addSubWindow(newDocument, Qt::Widget | Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint);
    subWindow->show();
}

void MainWindow::closeDocument()
{
    m_mdiArea->closeActiveSubWindow();
}

void MainWindow::closeAllDocuments()
{
    int count = m_mdiArea->subWindowList().count();

    while(count){
        m_mdiArea->closeActiveSubWindow();

        // If the sub window did not close (user canceled action), we stop here
        if(count == m_mdiArea->subWindowList().count())
            break;

        count--;
    }
}

void MainWindow::documentClosed()
{
    if(m_mdiArea->subWindowList().empty()){
        m_layersArea->takeWidget();

        m_actions["close"]->setDisabled(true);
        m_actions["close all"]->setDisabled(true);
        m_actions["save"]->setDisabled(true);
        m_actions["save as"]->setDisabled(true);

        m_actions["new layer"]->setDisabled(true);
        m_actions["remove layer"]->setDisabled(true);

        m_actions["select all"]->setDisabled(true);
        m_actions["deselect"]->setDisabled(true);
        m_actions["reselect"]->setDisabled(true);
        m_actions["inverse"]->setDisabled(true);
        m_menus["modify"]->setDisabled(true);
    }
}

void MainWindow::toolSelected(QAction *action)
{
    Tool::Name tool = static_cast<Tool::Name>(action->property("tool").toInt());

    if(m_tool == m_tools[tool])
        return;

    m_tool = m_tools[tool];

    m_tool->getToolBar(this)->show();
    this->addToolBar(m_tool->getToolBar(this));
    this->removeToolBar(m_toolOptions);
    m_toolOptions = m_tool->getToolBar(this);

    Workspace * workspace = this->getWorkspace();
    if(workspace != 0)
        workspace->setCursor(*m_tool->getCursor());
}

void MainWindow::cursorChanged(QCursor *cursor)
{
    this->getWorkspace()->setCursor(*cursor);
}

void MainWindow::saveDocument()
{
    this->getWorkspace()->save();
}

void MainWindow::saveDocumentAs()
{
    this->getWorkspace()->saveAs();
}

void MainWindow::cut()
{
    m_clipboard = this->getWorkspace()->getCanvas()->cut();
    m_actions["paste"]->setEnabled(true);
}

void MainWindow::copy()
{
    m_clipboard = this->getWorkspace()->getCanvas()->copy();
    m_actions["paste"]->setEnabled(true);
}

void MainWindow::paste()
{
    this->getWorkspace()->getCanvas()->paste(m_clipboard);
    this->selectionChanged(false);
}

void MainWindow::clear()
{
    this->getWorkspace()->getCanvas()->clear();
}

void MainWindow::createNewLayer()
{
    LayerStack *layerStack = (LayerStack*) m_layersArea->widget();
    layerStack->newLayer();
}

void MainWindow::removeLayer()
{
    LayerStack *layerStack = (LayerStack*) m_layersArea->widget();
    layerStack->removeLayer();
}

void MainWindow::selectAll()
{
    this->getWorkspace()->getCanvas()->selectAll();

    m_actions["deselect"]->setEnabled(true);
    m_actions["reselect"]->setDisabled(true);
    m_actions["inverse"]->setEnabled(true);
    m_menus["modify"]->setEnabled(true);
}

void MainWindow::deselect()
{
    this->getWorkspace()->getCanvas()->toggleSelection();

    m_actions["deselect"]->setDisabled(true);
    m_actions["reselect"]->setEnabled(true);
    m_actions["inverse"]->setDisabled(true);
    m_menus["modify"]->setDisabled(true);
}

void MainWindow::reselect()
{
    this->getWorkspace()->getCanvas()->toggleSelection();

    m_actions["deselect"]->setEnabled(true);
    m_actions["reselect"]->setDisabled(true);
    m_actions["inverse"]->setEnabled(true);
    m_menus["modify"]->setEnabled(true);
}

void MainWindow::inverse()
{
    this->getWorkspace()->getCanvas()->inverseSelection();
}

void MainWindow::selectionChanged(bool activated)
{
    m_actions["deselect"]->setEnabled(activated);
    m_actions["reselect"]->setDisabled(activated);
    m_actions["inverse"]->setEnabled(activated);
    m_menus["modify"]->setEnabled(activated);

    m_actions["cut"]->setEnabled(activated);
    m_actions["copy"]->setEnabled(activated);
    m_actions["clear"]->setEnabled(activated);
}

void MainWindow::documentActivated(QMdiSubWindow *subWindow)
{
    if(subWindow == 0){
        m_layersArea->setWidget(0);
        return;
    }

    Workspace *workspace = this->getWorkspace();
    LayerStack *layers = workspace->getLayerStack();
    m_layersArea->takeWidget();
    m_layersArea->setWidget(layers);
    workspace->setCursor(*m_tool->getCursor());

    m_actions["close"]->setEnabled(true);
    m_actions["close all"]->setEnabled(true);
    m_actions["save"]->setEnabled(true);
    m_actions["save as"]->setEnabled(true);
    m_actions["new layer"]->setEnabled(true);
    m_actions["remove layer"]->setEnabled(true);
    m_actions["select all"]->setEnabled(true);

    bool activated = workspace->getCanvas()->isSelectionActivated();

    m_actions["deselect"]->setEnabled(activated);
    m_actions["reselect"]->setDisabled(activated);
    m_actions["inverse"]->setEnabled(activated);
    m_menus["modify"]->setEnabled(activated);

    m_actions["cut"]->setEnabled(activated);
    m_actions["copy"]->setEnabled(activated);
    m_actions["clear"]->setEnabled(activated);
}