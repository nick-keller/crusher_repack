#include "selectiontool.h"

SelectionTool::SelectionTool(ColorPicker* colorPicker, Name toolName, QString name, QString icon, QString shortcut) :
    Tool(colorPicker, toolName, name, icon, shortcut), m_moving(false), m_drawing(false), m_unmodifiedMode(0)
{
}

void SelectionTool::createToolBar()
{
    this->createSelectionModeSelector();
}

void SelectionTool::mousePressEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(*useSelection && m_selectionMode == "only" && QColor(selection->pixel(mouse.getPos())) == Qt::white){
        m_moving = true;

        m_movingSelection = QImage(layer->size(), QImage::Format_ARGB32);
        m_movingSelection.fill(GREEN_SELECTION);
        QPainter painter(&m_movingSelection);
        painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        painter.drawImage(0, 0, *selection);
    }
    else if(mouse.button() == Qt::LeftButton){
        m_drawing = true;
        m_noKeyReleased = true;
        m_keysDown.remove(Qt::Key_Shift);
        m_keysDown.remove(Qt::Key_Alt);
    }
}

void SelectionTool::mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(m_moving){
        hud->fill(Qt::transparent);

        QPainter painter(hud);
        painter.setOpacity(.6);
        painter.drawImage(mouse.delta(), m_movingSelection);
    }
    else if(m_drawing){
        hud->fill(Qt::transparent);

        QPixmap temp(hud->size());
        temp.fill(Qt::transparent);

        QPainter painter(&temp);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(GREEN_SELECTION));

        this->paintSelection(mouse, &painter);

        painter.end();
        painter.begin(hud);
        painter.setOpacity(.6);
        painter.drawPixmap(0, 0, temp);
    }
    else{
        if(*useSelection && m_selectionMode == "only" && QColor(selection->pixel(mouse.getPos())) == Qt::white)
            this->setCursor(Selection);
        else
            this->setCorrectCursor();
    }
}

void SelectionTool::mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(m_moving){
        m_moving = false;

        selection->fill(Qt::transparent);
        hud->fill(Qt::transparent);

        QPainter painter(selection);
        painter.drawImage(mouse.delta(), m_movingSelection);
        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(Qt::white));
        painter.drawRect(0, 0, layer->width(), layer->height());
    }
    else if(m_drawing){
        m_drawing = false;

        if(m_selectionMode == "only" || *useSelection == false)
            selection->fill(Qt::transparent);

        *useSelection = true;
        hud->fill(Qt::transparent);

        QPixmap temp(layer->size());
        temp.fill(Qt::transparent);

        QPainter painter(&temp);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(Qt::white));

        this->paintSelection(mouse, &painter);

        painter.end();
        painter.begin(selection);

        if(m_selectionMode == "not")
            painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);

        if(m_selectionMode == "and")
            painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);

        painter.drawPixmap(0, 0, temp);

        if(m_noKeyReleased && m_unmodifiedMode != 0){
            if(m_selectionMode == "or" || m_selectionMode == "and")
                m_keysDown.insert(Qt::Key_Shift);
            if(m_selectionMode == "not" || m_selectionMode == "and")
                m_keysDown.insert(Qt::Key_Alt);
        }

        this->updateMode();
    }

    emit selectionChanged(true);
}

void SelectionTool::keyPressEvent(Qt::Key key)
{
    Tool::keyPressEvent(key);

    if(key == Qt::Key_Shift || key == Qt::Key_Alt)
        this->updateMode();
}

void SelectionTool::keyReleaseEvent(Qt::Key key)
{
    Tool::keyReleaseEvent(key);

    if(key == Qt::Key_Shift || key == Qt::Key_Alt){
        m_noKeyReleased = false;
        this->updateMode();
    }
}

void *SelectionTool::createSelectionModeSelector()
{
    m_only = m_toolbar->addAction(QIcon(":/icons/only.png"), "New selection");
    m_or = m_toolbar->addAction(QIcon(":/icons/or.png"), "Add to selection");
    m_not = m_toolbar->addAction(QIcon(":/icons/not.png"), "Subtract from selection");
    m_and = m_toolbar->addAction(QIcon(":/icons/and.png"), "Intersection with the selection");

    m_only->setCheckable(true);
    m_or->setCheckable(true);
    m_not->setCheckable(true);
    m_and->setCheckable(true);

    m_only->setProperty("mode", "only");
    m_or->setProperty("mode", "or");
    m_not->setProperty("mode", "not");
    m_and->setProperty("mode", "and");

    QActionGroup *selector = new QActionGroup(m_toolbar);
    selector->addAction(m_only);
    selector->addAction(m_or);
    selector->addAction(m_not);
    selector->addAction(m_and);

    QObject::connect(selector, SIGNAL(triggered(QAction*)), this, SLOT(changeSelectionMode(QAction*)));

    m_only->trigger();
}

void SelectionTool::setCorrectCursor()
{
    if(m_selectionMode == "only")
        this->setCursor(Cross);
    if(m_selectionMode == "or")
        this->setCursor(CrossPlus);
    if(m_selectionMode == "not")
        this->setCursor(CrossMinus);
    if(m_selectionMode == "and")
        this->setCursor(CrossAnd);
}

void SelectionTool::updateMode()
{
    if(!m_drawing && !m_moving){
        if(m_unmodifiedMode == 0){
            qDebug() << m_selectionMode;
            if(m_selectionMode == "only")
                m_unmodifiedMode = m_only;
            if(m_selectionMode == "or")
                m_unmodifiedMode = m_or;
            if(m_selectionMode == "not")
                m_unmodifiedMode = m_not;
            if(m_selectionMode == "and")
                m_unmodifiedMode = m_and;
        }

        if(m_keysDown.contains(Qt::Key_Shift) && m_keysDown.contains(Qt::Key_Alt))
            m_and->trigger();
        else if(m_keysDown.contains(Qt::Key_Shift))
            m_or->trigger();
        else if(m_keysDown.contains(Qt::Key_Alt))
            m_not->trigger();
        else{
            m_unmodifiedMode->trigger();
            m_unmodifiedMode = 0;
        }
    }
}

void SelectionTool::changeSelectionMode(QAction *action)
{
    m_selectionMode = action->property("mode").toString();

    this->setCorrectCursor();
}
