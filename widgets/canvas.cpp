#include "canvas.h"
#include "mainwindow.h"

Canvas::Canvas(MainWindow *mainWindow, QWidget *parent) :
    QWidget(parent), m_penGrid(BORDER), m_brushBackground(), m_mainWindow(mainWindow),
    m_selection(SCREEN_WIDTH, SCREEN_HEIGHT, QImage::Format_ARGB32), m_selectionActivated(false)
{
    m_layers = new LayerStack;
    this->initPaintDevice();
    this->setMouseTracking(true);

    QObject::connect(m_layers, SIGNAL(modified()), this, SLOT(update()));
}

void Canvas::initPaintDevice()
{
    this->zoom(INIT_ZOOM_LEVEL);

    m_penGrid.setCosmetic(true);
    m_penGrid.setWidth(1);

    // Make a checker board pattern
    QPixmap backgroundTexture(16, 16);
    QPainter texturePainter(&backgroundTexture);
    texturePainter.setPen(Qt::NoPen);
    texturePainter.setBrush(QBrush(BACKGROUND));

    backgroundTexture.fill(Qt::white);
    texturePainter.drawRect(0, 0, 8, 8);
    texturePainter.drawRect(8, 8, 8, 8);
    m_brushBackground.setTexture(backgroundTexture);

    // Make selection brush
    QPixmap selectionTexture(":/icons/selection.png");
    m_selectionPen = QPen(QBrush(selectionTexture), 1);
    m_selectionPen.setCosmetic(true);

    // Init selection
    m_selection.fill(Qt::white);
}

bool Canvas::isPointSelected(int x, int y)
{
    return QColor(m_selection.pixel(x, y)) == Qt::white;
}

void Canvas::modifySelection(QImage *selection, int radius, bool sharp, QColor expand)
{
    int size = radius *2 +1;
    QImage element(size, size, selection->format());
    element.fill(Qt::transparent);

    if(sharp)
        element.fill(Qt::white);
    else{
        QPainter painter(&element);
        painter.setPen(QPen(Qt::white));
        Tool::drawPerfectEllipse(&painter, 0, 0, size -1, size -1, true);
    }

    modifySelection(selection, element, expand);
}

void Canvas::modifySelection(QImage *selection, QImage element, QColor expand)
{
    QImage newSelection(selection->width() +2, selection->height() +2, selection->format());
    newSelection.fill(Qt::transparent);
    QPainter painter(&newSelection);
    painter.drawImage(1, 1, *selection);
    painter.end();

    painter.begin(selection);
    if(expand != Qt::white)
        painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);

    for(int i(0); i < newSelection.width(); ++i){
        for(int j(0); j < newSelection.height(); ++j){
            if(newSelection.pixel(i, j) == expand.rgba()){
                painter.drawImage(i - element.width() /2 -1 +(element.width() %2 == 0 && expand == Qt::white ? 1: 0), j - element.height() /2 -1 +(element.height() %2 == 0 && expand == Qt::white ? 1: 0), element);
            }
        }
    }
}

QRect Canvas::getRealRect(QImage *image)
{
    QRect rect;
    int minX = SCREEN_WIDTH, minY = SCREEN_HEIGHT, maxX = 0, maxY = 0;

    for(int i(0); i < image->width(); ++i){
        for(int j(0); j < image->height(); ++j){
            if(qAlpha(image->pixel(i, j)) != 0){
                minX = std::min(minX, i);
                maxX = std::max(maxX, i);
                minY = std::min(minY, j);
                maxY = std::max(maxY, j);
            }
        }
    }

    rect.setTop(minY);
    rect.setBottom(maxY);
    rect.setLeft(minX);
    rect.setRight(maxX);

    return rect;
}

void Canvas::zoom(float level)
{
    if(level < 1 || level > 100) return;

    this->setFixedSize(SCREEN_WIDTH * level +1, SCREEN_HEIGHT * level +1);
    m_zoomLevel = level;
}

void Canvas::toggleSelection()
{
    m_selectionActivated = !m_selectionActivated;
    update();
}

void Canvas::selectAll()
{
    m_selection.fill(Qt::white);
    m_selectionActivated = true;
    update();
}

void Canvas::inverseSelection()
{
    QPixmap temp(m_selection.size());
    temp.fill(Qt::transparent);
    QPainter painter(&temp);
    painter.setPen(QPen(Qt::white));

    for(int i(0); i < SCREEN_WIDTH; ++i)
        for(int j(0); j < SCREEN_HEIGHT; ++j)
            if(!this->isPointSelected(i, j))
                painter.drawPoint(i, j);

    painter.end();
    m_selection.fill(Qt::transparent);
    painter.begin(&m_selection);
    painter.drawPixmap(0, 0, temp);

    update();
}

void Canvas::expandSelection(int radius, bool sharp)
{
    this->modifySelection(&m_selection, radius, sharp, Qt::white);
}

void Canvas::contractSelection(int radius, bool sharp)
{
    this->modifySelection(&m_selection, radius, sharp, Qt::transparent);
}

QImage Canvas::cut()
{
    QImage temp = this->copy();

    QPainter painter(m_layers->getImage());
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    painter.drawImage(0, 0, m_selection);

    update();
    m_layers->update();

    return temp;
}

QImage Canvas::copy()
{
    QImage temp(SCREEN_WIDTH, SCREEN_HEIGHT, QImage::Format_ARGB32);
    temp.fill(Qt::transparent);

    QPainter painter(&temp);
    painter.drawImage(0, 0, m_selection);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.drawImage(0, 0, *m_layers->getImage());
    painter.end();

    return temp;
}

void Canvas::paste(QImage image)
{
    m_layers->newLayer();

    QPainter painter(m_layers->getImage());
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawImage(0, 0, image);

    m_selectionActivated = false;
    update();
}

void Canvas::clear()
{
    QPainter painter(m_layers->getImage());
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    painter.drawImage(0, 0, m_selection);

    update();
    m_layers->update();
}

void Canvas::fillSelection(QBrush brush)
{
    QImage temp(m_selection.size(), m_selection.format());
    temp.fill(Qt::transparent);

    QPainter painter(&temp);
    painter.drawImage(0, 0, m_selection);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.setPen(QPen(Qt::NoPen));
    painter.setBrush(brush);
    painter.drawRect(0, 0, temp.width(), temp.height());
    painter.end();

    painter.begin(m_layers->getImage());
    painter.drawImage(0, 0, temp);

    update();
    m_layers->update();
}

void Canvas::rotateLayer(int angle)
{
    QImage *layer = m_layers->getImage();
    QImage temp = *layer;
    layer->fill(Qt::transparent);

    QRect originalRect = Canvas::getRealRect(&temp);

    QPainter painter(layer);
    painter.translate(layer->rect().center());
    painter.rotate(angle);
    painter.translate(-originalRect.center());
    painter.drawImage(0, 0, temp);

    QRect newRect = Canvas::getRealRect(layer);
    QPoint offset = newRect.topLeft();
    newRect.moveCenter(originalRect.center());
    newRect.moveLeft(std::max(0, newRect.left()));
    newRect.moveRight(std::min(SCREEN_WIDTH -1, newRect.right()));
    newRect.moveTop(std::max(0, newRect.top()));
    newRect.moveBottom(std::min(SCREEN_HEIGHT -1, newRect.bottom()));

    offset = newRect.topLeft() - offset;

    temp = *layer;
    layer->fill(Qt::transparent);

    painter.resetTransform();
    painter.drawImage(offset, temp);

    update();
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(m_penGrid);
    painter.setBrush(m_brushBackground);

    // Background
    if(m_zoomLevel > GRID_MIN_ZOOM_LEVEL){
        painter.save();
            painter.scale(m_zoomLevel /4, m_zoomLevel /4);
            painter.drawRect(0, 0, SCREEN_WIDTH *4, SCREEN_HEIGHT *4);
        painter.restore();

        painter.scale(m_zoomLevel, m_zoomLevel);
    }
    else {
        painter.drawRect(0, 0, this->width()-1, this->height()-1);
        painter.scale(m_zoomLevel, m_zoomLevel);
    }

    // Layers
    for(int i(0); i < m_layers->count(); ++i)
        if(m_layers->isLayerVisible(i))
            painter.drawImage(0, 0, *m_layers->getImage(i));

    painter.drawImage(0, 0, *m_layers->getHud());

    // Grid
    if(m_zoomLevel > GRID_MIN_ZOOM_LEVEL){
        for(int i(1); i < SCREEN_WIDTH; ++i)
            painter.drawLine(i, 0, i, SCREEN_HEIGHT);
        for(int i(1); i < SCREEN_HEIGHT; ++i)
            painter.drawLine(0, i, SCREEN_WIDTH, i);
    }

    // Selection
    if(m_selectionActivated){
        painter.setPen(m_selectionPen);
        for(int i(0); i < SCREEN_WIDTH; ++i){
            for(int j(0); j < SCREEN_HEIGHT; ++j){

                if(this->isPointSelected(i, j)){
                    if(j == 0 || !this->isPointSelected(i, j -1))
                        painter.drawLine(i, j, i+1, j);

                    if(i == 0 || !this->isPointSelected(i-1, j))
                        painter.drawLine(i, j, i, j+1);

                    if(j == SCREEN_HEIGHT -1)
                        painter.drawLine(i, SCREEN_HEIGHT, i+1, SCREEN_HEIGHT);

                    if(i == SCREEN_WIDTH -1)
                        painter.drawLine(SCREEN_WIDTH, j, SCREEN_WIDTH, j+1);

                }
                else{
                    if(this->isPointSelected(i, j -1))
                        painter.drawLine(i, j, i+1, j);

                    if(this->isPointSelected(i -1, j))
                        painter.drawLine(i, j, i, j+1);
                }
            }
        }
    }

    if(!m_selectionActivated && m_selectionTimer.isActive())
        m_selectionTimer.stop();

    if(m_selectionActivated && !m_selectionTimer.isActive())
        m_selectionTimer.start(100, this);
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    m_mouse.handleEvent(event, m_zoomLevel);
    if(!m_mouse.moved()) return;

    m_mainWindow->getTool()->mouseMoveEvent(
                m_mouse,
                m_layers->getImage(),
                m_layers->getHud(),
                &m_selection,
                &m_selectionActivated);

    update();
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    m_mouse.handleEvent(event, m_zoomLevel);

    m_mainWindow->getTool()->mousePressEvent(
                m_mouse,
                m_layers->getImage(),
                m_layers->getHud(),
                &m_selection,
                &m_selectionActivated);

    update();
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    m_mouse.handleEvent(event, m_zoomLevel);

    m_mainWindow->getTool()->mouseReleaseEvent(
                m_mouse,
                m_layers->getImage(),
                m_layers->getHud(),
                &m_selection,
                &m_selectionActivated);

    m_layers->update();
    update();
}

void Canvas::leaveEvent(QEvent *event)
{
    m_layers->getHud()->fill(Qt::transparent);
    update();
}

void Canvas::timerEvent(QTimerEvent *event)
{
    QPixmap oldTexture = m_selectionPen.brush().texture();
    QPixmap texture(oldTexture.size());
    QPainter painter(&texture);
    QBrush brush;

    painter.drawPixmap(1, 0, oldTexture);
    painter.drawPixmap(-oldTexture.width() +1, 0, oldTexture);

    brush = QBrush(texture);
    brush.setTransform(QTransform().scale(1. / (float) m_zoomLevel, 1. / (float) m_zoomLevel));
    m_selectionPen = QPen(brush, 1);
    m_selectionPen.setCosmetic(true);
    update();
}
