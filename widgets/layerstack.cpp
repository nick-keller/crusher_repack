#include "layerstack.h"

LayerStack::LayerStack(QWidget *parent) :
    QWidget(parent), m_selectedLayer(-1), m_movingLayers(false)
{
    this->newLayer();
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    this->setCursor(Qt::PointingHandCursor);
    this->getImage()->fill(WHITE);
}

int LayerStack::isLayerVisible(int i)
{
    return m_layers[i].getVisible();
}

void LayerStack::setOnlyLayer(QImage image)
{
    QPainter painter(this->getImage());
    painter.drawImage(0, 0, image);
}

QImage LayerStack::getFlattenedImage()
{
    QImage image(m_layers[0].getImage()->size(), m_layers[0].getImage()->format());
    image.fill(WHITE);

    QPainter painter(&image);

    for(int i(0); i < m_layers.count(); ++i)
        if(m_layers[i].getVisible())
            painter.drawImage(0, 0, *m_layers[i].getImage());

    return image;
}

void LayerStack::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.save();

    for(int i(m_layers.count()); i--;){
        m_layers[i].paint(painter, this->width(), i == m_selectedLayer);
        painter.translate(0, LAYER_HEIGHT);
    }

    painter.restore();

    if(m_movingLayers){
        painter.setPen(QPen(QColor(41, 41, 41)));
        painter.setBrush(QBrush(QColor(158, 158, 158)));

        // Separator
        painter.drawRect(0, this->getMidLayerIndex() * LAYER_HEIGHT -2, this->width() -1, 2);

        // Layer
        painter.translate(0, m_mouse.y() - m_mouse.clickedY()%LAYER_HEIGHT);
        painter.setOpacity(.5);
        m_layers[m_selectedLayer].paint(painter, this->width(), true);
    }
}

void LayerStack::mouseMoveEvent(QMouseEvent *event)
{
    m_mouse.handleEvent(event);

    // If we get away from where we clicked then we engage layer moving
    if(!this->clickedInEyeArea() && this->isMouseDown() && m_mouse.getClickedRect().height() > 3){
        this->setCursor(Qt::ClosedHandCursor);
        m_movingLayers = true;
    }

    // If we are toggling multiple layers
    if(this->clickedInEyeArea() && this->isMouseDown() && this->isInEyeArea()){
        m_layers[this->getLayerIndex()].setVisible(m_layers[this->getLayerIndex(m_mouse.clickedY())].getVisible());

        emit modified();
        update();
    }

    if(m_movingLayers)
        update();
}

void LayerStack::mouseReleaseEvent(QMouseEvent *event)
{
    m_mouse.handleEvent(event);

    if(m_movingLayers){
        this->setCursor(Qt::PointingHandCursor);
        m_movingLayers = false;

        int moveTo = m_layers.count() - getMidLayerIndex() -1;
        if(moveTo < m_selectedLayer)
            moveTo++;

        moveTo = std::max(0, std::min(moveTo, m_layers.count()-1));

        m_layers.move(m_selectedLayer, moveTo);
        m_selectedLayer = moveTo;

        emit modified();
    }

    update();
}

void LayerStack::mousePressEvent(QMouseEvent *event)
{
    m_mouse.handleEvent(event);

    if(!this->isInEyeArea())
        m_selectedLayer = this->getLayerIndex();
    else{
        m_layers[this->getLayerIndex()].toggleVisible();
        emit modified();
    }

    update();
}

void LayerStack::updateHeight()
{
    this->setFixedHeight(LAYER_HEIGHT * m_layers.count() +1);
}

int LayerStack::getMidLayerIndex(int y)
{
    y += LAYER_HEIGHT/2;
    return std::max(0, std::min(m_layers.count(), y / LAYER_HEIGHT));
}

int LayerStack::getMidLayerIndex()
{
    return this->getMidLayerIndex(m_mouse.y());
}

int LayerStack::getLayerIndex(int y)
{
    return std::max(0, std::min(m_layers.count()-1, m_layers.count() - (y-1)/LAYER_HEIGHT -1));
}

int LayerStack::getLayerIndex()
{
    return this->getLayerIndex(m_mouse.y());
}

bool LayerStack::isInEyeArea(int x)
{
    return x < LAYER_EYE_WIDTH;
}

bool LayerStack::isInEyeArea()
{
    return this->isInEyeArea(m_mouse.x());
}

bool LayerStack::clickedInEyeArea()
{
    return this->isInEyeArea(m_mouse.clickedX());
}

bool LayerStack::isMouseDown()
{
    return m_mouse.isButtonDown(Qt::LeftButton);
}

void LayerStack::newLayer()
{
    m_layers.insert(++m_selectedLayer, Layer(m_layers.count()));
    this->updateHeight();
}

void LayerStack::removeLayer()
{
    if(m_layers.count() > 1){
        m_layers.removeAt(m_selectedLayer);
        m_selectedLayer = std::min(m_layers.count() -1, m_selectedLayer);
        this->updateHeight();
        emit modified();
    }
}
