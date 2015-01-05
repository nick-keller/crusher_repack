#include "texttool.h"

TextTool::TextTool(ColorPicker *colorPicker) : Tool(colorPicker, Tool::Text, "Text Tool (T)", ":/tools/text.png", "T")
{
    QFontDatabase database;
    QFontDatabase::addApplicationFont(":/fonts/munro_small.ttf"); // 10
    QFontDatabase::addApplicationFont(":/fonts/sanstaina.ttf"); // 16
    QFontDatabase::addApplicationFont(":/fonts/PressStart2P.ttf"); // 8
    QFontDatabase::addApplicationFont(":/fonts/Fipps-Regular.otf"); // 8
    QFontDatabase::addApplicationFont(":/fonts/thin_pixel-7.ttf"); // 20
    QFontDatabase::addApplicationFont(":/fonts/Minecraftia-Regular.ttf"); // 8
    QFontDatabase::addApplicationFont(":/fonts/slkscr.ttf"); // 9
    QFontDatabase::addApplicationFont(":/fonts/pixelmix.ttf"); // 8
    QFontDatabase::addApplicationFont(":/fonts/pixelmix_bold.ttf"); // 8
    QFontDatabase::addApplicationFont(":/fonts/visitor.ttf"); // 12
    QFontDatabase::addApplicationFont(":/fonts/wendy.ttf"); // 10
}

void TextTool::mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    QFontMetrics metrics(m_font);
    QRect rect = metrics.boundingRect(m_text->text());

    QPainter *painter = this->getPainter(layer);
    painter->setFont(m_font);
    painter->drawText(mouse.getPos() - rect.center(), m_text->text());
    delete painter;
}

void TextTool::mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    hud->fill(Qt::transparent);

    QFontMetrics metrics(m_font);
    QRect rect = metrics.boundingRect(m_text->text());

    QPainter *painter = this->getPainter(hud);
    painter->setFont(m_font);
    painter->drawText(mouse.getPos() - rect.center(), m_text->text());
    delete painter;
}

void TextTool::setFont(QString name)
{
    m_font = QFont(name);
    m_font.setStyleStrategy(QFont::NoAntialias);
    m_font.setPixelSize(m_fontSelector->currentData().toInt());
}

void TextTool::createToolBar()
{
    QLabel *labelFont = new QLabel("Font: ");

    m_fontSelector = new QComboBox(m_toolbar);
    m_fontSelector->addItem("Fipps", 8);
    m_fontSelector->addItem("Minecraftia", 8);
    m_fontSelector->addItem("Munro Small", 10);
    m_fontSelector->addItem("Pixelmix", 8);
    m_fontSelector->addItem("Pixelmix Bold", 8);
    m_fontSelector->addItem("Press Start 2P", 8);
    m_fontSelector->addItem("SansTaiNa", 16);
    m_fontSelector->addItem("Silkscreen", 9);
    m_fontSelector->addItem("Thin Pixel-7", 20);
    m_fontSelector->addItem("Visitor TT2 BRK", 12);
    m_fontSelector->addItem("Wendy", 10);

    QLabel *labelText = new QLabel("Text: ");

    m_text = new QLineEdit();
    m_text->setMaximumWidth(300);

    m_toolbar->addWidget(labelFont);
    m_toolbar->addWidget(m_fontSelector);
    this->createSpacer();
    m_toolbar->addWidget(labelText);
    m_toolbar->addWidget(m_text);

    QObject::connect(m_fontSelector, SIGNAL(currentTextChanged(QString)), this, SLOT(setFont(QString)));

    m_fontSelector->setCurrentIndex(1);
    m_fontSelector->setCurrentIndex(0);
}
