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
    this->createDialogs();
    this->selectionChanged(false);
    this->documentClosed();
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
    m_actions.insert("open", menuFile->addAction("Open...", this, SLOT(openDocument()), QString("Ctrl+O")));
    menuFile->addSeparator();
    m_actions.insert("close", menuFile->addAction("Close", this, SLOT(closeDocument()), QString("Ctrl+W")));
    m_actions.insert("close all", menuFile->addAction("Close all", this, SLOT(closeAllDocuments()), QString("Alt+Ctrl+W")));
    m_actions.insert("save", menuFile->addAction("Save", this, SLOT(saveDocument()), QString("Ctrl+S")));
    m_actions.insert("save as", menuFile->addAction("Save as...", this, SLOT(saveDocumentAs()), QString("Ctrl+Shift+S")));
    menuFile->addSeparator();
    m_actions.insert("exit", menuFile->addAction("Exit", this, SLOT(close()), QString("Ctrl+Q")));

    m_actionsDocOpened
            << m_actions["close"]
            << m_actions["close all"]
            << m_actions["save"]
            << m_actions["save as"];

    // Edit ------------------------------------------------------------------------------
    QMenu *menuEdit = menuBar->addMenu("&Edit");

    m_actions.insert("cut", menuEdit->addAction("Cut", this, SLOT(cut()), QString("Ctrl+X")));
    m_actions.insert("copy", menuEdit->addAction("Copy", this, SLOT(copy()), QString("Ctrl+C")));
    m_actions.insert("paste", menuEdit->addAction("Paste", this, SLOT(paste()), QString("Ctrl+V")));
    m_actions.insert("clear", menuEdit->addAction("Clear", this, SLOT(clear()), QString("Del")));
    menuEdit->addSeparator();
    m_actions.insert("fill", menuEdit->addAction("Fill...", this, SLOT(fill()), QString("Shift+F5")));

    m_actions["paste"]->setDisabled(true);

    m_actionsSelectionActivated
            << m_actions["cut"]
            << m_actions["copy"]
            << m_actions["clear"]
            << m_actions["fill"];

    // Layer -----------------------------------------------------------------------------
    QMenu *menuLayer = menuBar->addMenu("&Layer");

    m_actions.insert("new layer", menuLayer->addAction(QIcon(":/icons/newlayer.png"), "New layer...", this, SLOT(createNewLayer()),  QString("Shift+Ctrl+N")));
    m_actions.insert("remove layer", menuLayer->addAction(QIcon(":/icons/trash.png"), "Remove layer", this, SLOT(removeLayer())));

    m_actionsDocOpened
            << m_actions["new layer"]
            << m_actions["remove layer"];

    // Select ----------------------------------------------------------------------------
    QMenu *menuSelect = menuBar->addMenu("&Select");

    m_actions.insert("select all", menuSelect->addAction("All", this, SLOT(selectAll()), QString("Ctrl+A")));
    m_actions.insert("deselect", menuSelect->addAction("Deselect", this, SLOT(deselect()), QString("Ctrl+D")));
    m_actions.insert("reselect", menuSelect->addAction("Reselect", this, SLOT(reselect()), QString("Ctrl+Shift+D")));
    m_actions.insert("inverse", menuSelect->addAction("Inverse", this, SLOT(inverse()), QString("Ctrl+Shift+I")));
    menuSelect->addSeparator();

    m_menus.insert("modify", menuSelect->addMenu("Modify"));

    m_actions.insert("expand", m_menus["modify"]->addAction("Expand...", this, SLOT(expand())));
    m_actions.insert("contract", m_menus["modify"]->addAction("Contract...", this, SLOT(contract())));
    m_actions.insert("smooth selection", m_menus["modify"]->addAction("Smooth...", this, SLOT(smoothSelection())));

    m_actionsDocOpened << m_actions["select all"];
    m_actionsSelectionActivated << m_actions["deselect"] << m_actions["inverse"];
    m_actionsSelectionNotActivated << m_actions["reselect"];
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
    m_tools.insert(Tool::WandTool, new WandTool(m_colorPicker));
    m_tools.insert(Tool::Rect, new RectTool(m_colorPicker));
    m_tools.insert(Tool::Ellipse, new EllipseTool(m_colorPicker));
    m_tools.insert(Tool::Line, new LineTool(m_colorPicker));
    m_tools.insert(Tool::BucketTool, new BucketTool(m_colorPicker));
    m_tools.insert(Tool::Gradient, new GradientTool(m_colorPicker));
    m_tools.insert(Tool::PatternPicker, new PatternPickerTool(m_colorPicker));

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
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(198 ,198, 198));

    m_layersArea = new QScrollArea;
    m_layersArea->setPalette(palette);
    m_layersArea->setWidgetResizable(true);
    m_layersArea->setMinimumWidth(250);

    QPushButton *newLayerButton = new QPushButton(QIcon(":/icons/newlayer.png"), "");
    QPushButton *removeLayerButton = new QPushButton(QIcon(":/icons/trash.png"), "");
    newLayerButton->setFlat(true);
    removeLayerButton->setFlat(true);

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

void MainWindow::createDialogs()
{
    m_dialogs.insert("expand", new ModifySelection("Expand", this));
    m_dialogs.insert("contract", new ModifySelection("Contract", this));
    m_dialogs.insert("smooth selection", new SmoothSelection(this));
    m_dialogs.insert("fill", new FillSelector(m_colorPicker, this));
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

    for(int i(0); i < m_actionsDocOpened.size(); ++i)
        m_actionsDocOpened[i]->setEnabled(true);

    for(int i(0); i < m_actionsSelectionActivated.size(); ++i)
        m_actionsSelectionActivated[i]->setEnabled(false);

    for(int i(0); i < m_actionsSelectionNotActivated.size(); ++i)
        m_actionsSelectionNotActivated[i]->setEnabled(true);
}

void MainWindow::openDocument()
{
    QSettings settings("crusher.ini", QSettings::IniFormat);

    QString fileName = QFileDialog::getOpenFileName(this, "Open", settings.value("path/open").toString(), "Any (*.crd *.png *.jpg);;Crusher Document (*.crd);;Image (*.png *.jpg)");

    if(fileName.isNull())
        return;

    QFileInfo fileInfo(fileName);
    settings.setValue("path/open", fileInfo.absoluteDir().absolutePath());

    if(fileInfo.suffix() == "crd"){

    }
    else{
        QImage file(fileName);
        ImportDialog dialog(file, this);


        QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog.exec());

        if(code == QDialog::Rejected)
            return;

        this->createNewDocument();
        this->getWorkspace()->getCanvas()->getLayerStack()->setOnlyLayer(dialog.getResult());
    }
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

        for(int i(0); i < m_actionsDocOpened.size(); ++i)
            m_actionsDocOpened[i]->setEnabled(false);

        for(int i(0); i < m_actionsSelectionActivated.size(); ++i)
            m_actionsSelectionActivated[i]->setEnabled(false);

        for(int i(0); i < m_actionsSelectionNotActivated.size(); ++i)
            m_actionsSelectionNotActivated[i]->setEnabled(false);

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
    if(this->getWorkspace() != 0)
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

void MainWindow::fill()
{
    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(m_dialogs["fill"]->exec());

    if(code == QDialog::Rejected)
        return;

    FillSelector *dialog = (FillSelector*) m_dialogs["fill"];
    this->getWorkspace()->getCanvas()->fillSelection(dialog->getBrush());
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
    this->selectionChanged(true);
}

void MainWindow::deselect()
{
    this->getWorkspace()->getCanvas()->toggleSelection();
    this->selectionChanged(false);
}

void MainWindow::reselect()
{
    this->getWorkspace()->getCanvas()->toggleSelection();
    this->selectionChanged(true);
}

void MainWindow::inverse()
{
    this->getWorkspace()->getCanvas()->inverseSelection();
}

void MainWindow::expand()
{
    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(m_dialogs["expand"]->exec());

    if(code == QDialog::Rejected)
        return;

    ModifySelection *dialog = (ModifySelection*) m_dialogs["expand"];
    this->getWorkspace()->getCanvas()->expandSelection(dialog->radius(), dialog->sharp());
}

void MainWindow::contract()
{
    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(m_dialogs["contract"]->exec());

    if(code == QDialog::Rejected)
        return;

    ModifySelection *dialog = (ModifySelection*) m_dialogs["contract"];
    this->getWorkspace()->getCanvas()->contractSelection(dialog->radius(), dialog->sharp());
}

void MainWindow::smoothSelection()
{
    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(m_dialogs["smooth selection"]->exec());

    if(code == QDialog::Rejected)
        return;

    SmoothSelection *dialog = (SmoothSelection*) m_dialogs["smooth selection"];
    this->getWorkspace()->getCanvas()->contractSelection(dialog->radius(), false);
    this->getWorkspace()->getCanvas()->expandSelection(dialog->radius(), false);
}

void MainWindow::selectionChanged(bool activated)
{
    for(int i(0); i < m_actionsSelectionActivated.size(); ++i)
        m_actionsSelectionActivated[i]->setEnabled(activated);

    for(int i(0); i < m_actionsSelectionNotActivated.size(); ++i)
        m_actionsSelectionNotActivated[i]->setDisabled(activated);

    m_menus["modify"]->setEnabled(activated);
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

    this->selectionChanged(workspace->getCanvas()->isSelectionActivated());
}
