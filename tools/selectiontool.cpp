#include "selectiontool.h"

SelectionTool::SelectionTool(ColorPicker* colorPicker, Name toolName, QString name, QString icon, QString shortcut) :
    Tool(colorPicker, toolName, name, icon, shortcut), m_moving(false)
{
}

void SelectionTool::mousePressEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(*useSelection && QColor(selection->pixel(mouse.getPos())) == Qt::white){
        m_moving = true;

        m_movingSelection = QImage(layer->size(), QImage::Format_ARGB32);
        m_movingSelection.fill(GREEN_SELECTION);
        QPainter painter(&m_movingSelection);
        painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        painter.drawImage(0, 0, *selection);
    }
    else{
        m_moving = false;
    }
}

void SelectionTool::createToolBar()
{
    this->createSelectionModeSelector();
}

void SelectionTool::mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(m_moving){
        QPoint delta = mouse.getPos() - mouse.getClickedAt();

        hud->fill(Qt::transparent);
        QPainter painter(hud);
        painter.setOpacity(.6);
        painter.drawImage(delta, m_movingSelection);
    }
    else if(mouse.isButtonDown(Qt::LeftButton)){
        hud->fill(Qt::transparent);
        QRect rect = mouse.getClickedRect();
        rect.setSize(QSize(rect.width() +1, rect.height() +1));
        QPixmap temp(hud->size());
        temp.fill(Qt::transparent);

        QPainter painterTemp(&temp);
        painterTemp.setPen(Qt::NoPen);
        painterTemp.setBrush(QBrush(GREEN_SELECTION));
        this->paintSelection(mouse, &painterTemp);

        QPainter painterHud(hud);
        painterHud.setOpacity(.6);
        painterHud.drawPixmap(0, 0, temp);
    }
    else{
        if(*useSelection && QColor(selection->pixel(mouse.getPos())) == Qt::white)
            this->setCursor(Selection);
        else
            this->setCorrectCursor();
    }
}

void SelectionTool::mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(m_moving){
        m_moving = false;
        QPoint delta = mouse.getPos() - mouse.getClickedAt();

        selection->fill(Qt::transparent);
        hud->fill(Qt::transparent);
        QPainter painter(selection);
        painter.drawImage(delta, m_movingSelection);
        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(Qt::white));
        painter.drawRect(0, 0, layer->width(), layer->height());
    }
    else{
        QRect rect = mouse.getClickedRect();
        rect.setSize(QSize(rect.width() +1, rect.height() +1));

        if(m_selectionMode == "only" || *useSelection == false)
            selection->fill(Qt::transparent);

        *useSelection = true;
        hud->fill(Qt::transparent);

        QPixmap temp(layer->size());
        temp.fill(Qt::transparent);
        QPainter painterTemp(&temp);
        painterTemp.setPen(Qt::NoPen);
        painterTemp.setBrush(QBrush(Qt::white));
        this->paintSelection(mouse, &painterTemp);

        QPainter painter(selection);

        if(m_selectionMode == "not")
            painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);

        if(m_selectionMode == "and")
            painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);

        painter.drawPixmap(0, 0, temp);
    }

    emit selectionChanged(true);
}

void *SelectionTool::createSelectionModeSelector()
{
    QAction *only = m_toolbar->addAction(QIcon(":/icons/only.png"), "New selection");
    QAction *or_ = m_toolbar->addAction(QIcon(":/icons/or.png"), "Add to selection");
    QAction *not_ = m_toolbar->addAction(QIcon(":/icons/not.png"), "Subtract from selection");
    QAction *and_ = m_toolbar->addAction(QIcon(":/icons/and.png"), "Intersection with the selection");

    only->setCheckable(true);
    or_->setCheckable(true);
    not_->setCheckable(true);
    and_->setCheckable(true);

    only->setProperty("mode", "only");
    or_->setProperty("mode", "or");
    not_->setProperty("mode", "not");
    and_->setProperty("mode", "and");

    QActionGroup *selector = new QActionGroup(m_toolbar);
    selector->addAction(only);
    selector->addAction(or_);
    selector->addAction(not_);
    selector->addAction(and_);

    QObject::connect(selector, SIGNAL(triggered(QAction*)), this, SLOT(changeSelectionMode(QAction*)));

    only->trigger();
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

void SelectionTool::changeSelectionMode(QAction *action)
{
    m_selectionMode = action->property("mode").toString();

    this->setCorrectCursor();
}
