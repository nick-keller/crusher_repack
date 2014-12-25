#include "gradienttool.h"

GradientTool::GradientTool(ColorPicker *colorPicker) : Tool(colorPicker, Tool::Gradient, "Gradient Tool", ":/tools/gradient.png")
{
}

void GradientTool::mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(mouse.isButtonDown(Qt::LeftButton)){
        hud->fill(Qt::transparent);

        QImage temp(layer->size(), layer->format());
        temp.fill(Qt::transparent);

        this->drawGradient(mouse, &temp, this->getColor(m_from->currentText()), this->getColor(m_to->currentText()));
        this->drawInSelection(hud, &temp, selection, useSelection);
    }
}

void GradientTool::mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    hud->fill(Qt::transparent);

    QImage temp(layer->size(), layer->format());
    temp.fill(Qt::transparent);

    this->drawGradient(mouse, &temp, this->getColor(m_from->currentText()), this->getColor(m_to->currentText()));
    this->drawInSelection(layer, &temp, selection, useSelection);
}

void GradientTool::createToolBar()
{
    QLabel *labelFrom = new QLabel("From: ", m_toolbar);
    m_from = new QComboBox(m_toolbar);
    m_from->addItem(QIcon(":/icons/black.png"), "Black");
    m_from->addItem(QIcon(":/icons/white.png"), "White");
    m_from->addItem(QIcon(":/icons/nofill.png"), "None");
    m_from->setCurrentIndex(1);

    m_toolbar->addWidget(labelFrom);
    m_toolbar->addWidget(m_from);
    this->createSpacer();

    QPushButton *swap = new QPushButton(QIcon(":/icons/swap.png"), "", m_toolbar);
    swap->setFlat(true);
    QObject::connect(swap, SIGNAL(clicked()), this, SLOT(swap()));

    m_toolbar->addWidget(swap);
    this->createSpacer();

    QLabel *labelTo = new QLabel("To: ", m_toolbar);
    m_to = new QComboBox(m_toolbar);
    m_to->addItem(QIcon(":/icons/black.png"), "Black");
    m_to->addItem(QIcon(":/icons/white.png"), "White");
    m_to->addItem(QIcon(":/icons/nofill.png"), "None");

    m_toolbar->addWidget(labelTo);
    m_toolbar->addWidget(m_to);
    this->createSpacer();

    m_toolbar->addSeparator();
    this->createSpacer();

    m_dithering = new QCheckBox("Dithering", m_toolbar);
    m_ditheringLevel = new QSpinBox(m_toolbar);
    m_ditheringLevel->setRange(1, 5);
    m_ditheringLevel->setValue(3);
    m_ditheringLevel->setFixedWidth(40);
    MagicLabel *labelLevel = new MagicLabel("Level: ", m_ditheringLevel, m_toolbar);

    m_toolbar->addWidget(m_dithering);
    this->createSpacer();
    m_toolbar->addWidget(labelLevel);
    m_toolbar->addWidget(m_ditheringLevel);

    this->updateBrushes();
    QObject::connect(m_from, SIGNAL(currentIndexChanged(int)), this, SLOT(updateBrushes()));
    QObject::connect(m_to, SIGNAL(currentIndexChanged(int)), this, SLOT(updateBrushes()));
}

void GradientTool::drawGradient(MouseState mouse, QImage *image, QColor from, QColor to)
{
    QPainter painter(image);

    QPoint gradVec = mouse.getPos() -  mouse.getClickedAt();
    float length = std::sqrt(gradVec.x() * gradVec.x() + gradVec.y() * gradVec.y());
    float vecX = gradVec.x() / length;
    float vecY = gradVec.y() / length;
    qsrand(0);

    for(int i(0); i < image->width(); ++i){
        for(int j(0); j < image->height(); ++j){
            QPoint point = QPoint(i, j) - mouse.getClickedAt();
            float val = (float)(point.x() * vecX + point.y() * vecY) / length;

            val = std::min(1.f, std::max(0.f, val));

            if(m_dithering->isChecked()){
                painter.setPen(QPen(m_brushes[(int) round(val * std::pow(2, m_ditheringLevel->value() -1)) * std::pow(2, 5-m_ditheringLevel->value())], 1));
            }
            else{
                if(qrand() %128 < val *128)
                    painter.setPen(QPen(to));
                else
                    painter.setPen(QPen(from));
            }

            painter.drawPoint(i, j);
        }
    }
}

QColor GradientTool::getColor(QString name)
{
    if(name == "Black")
        return QColor(40, 40, 40);
    if(name == "White")
        return QColor(247, 247, 247);

    return Qt::transparent;
}

void GradientTool::updateBrushes()
{
    QImage gradientPatterns(":/icons/gradient.png");
    QPixmap texture(4, 4);
    QPainter painter(&gradientPatterns);

    for(int i(0); i < gradientPatterns.width(); ++i){
        for(int j(0); j < gradientPatterns.height(); ++j){
            QColor color = QColor(gradientPatterns.pixel(i, j));

            painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
            painter.setPen(QPen(Qt::black));
            painter.drawPoint(i, j);

            if(color == Qt::white)
                painter.setPen(QPen(this->getColor(m_from->currentText())));
            else
                painter.setPen(QPen(this->getColor(m_to->currentText())));

            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
            painter.drawPoint(i, j);
        }
    }

    painter.end();
    painter.begin(&texture);
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    m_brushes.clear();

    for(int i(0); i < gradientPatterns.width()/4; ++i){
        painter.drawImage(-i*4, 0, gradientPatterns);
        m_brushes.append(QBrush(texture));
    }
}

void GradientTool::swap()
{
    int index = m_from->currentIndex();
    m_from->setCurrentIndex(m_to->currentIndex());
    m_to->setCurrentIndex(index);
}
