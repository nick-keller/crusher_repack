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

        this->drawGradient(mouse, &temp, this->getColor(m_from->currentText()), this->getColor(m_to->currentText()), false);
        this->drawInSelection(hud, &temp, selection, useSelection);
    }
}

void GradientTool::mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    hud->fill(Qt::transparent);

    QImage temp(layer->size(), layer->format());
    temp.fill(Qt::transparent);

    this->drawGradient(mouse, &temp, this->getColor(m_from->currentText()), this->getColor(m_to->currentText()), false);
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
    MagicLabel *labelLevel = new MagicLabel("Level: ", m_ditheringLevel, m_toolbar);

    m_toolbar->addWidget(m_dithering);
    this->createSpacer();
    m_toolbar->addWidget(labelLevel);
    m_toolbar->addWidget(m_ditheringLevel);

}

void GradientTool::drawGradient(MouseState mouse, QImage *image, QColor from, QColor to, bool dithering)
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

            val = 100* std::min(1.f, std::max(0.f, val));

            if(qrand() %100 < val)
                painter.setPen(QPen(to));
            else
                painter.setPen(QPen(from));

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
