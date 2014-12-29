#include "patternpickertool.h"

PatternPickerTool::PatternPickerTool(ColorPicker *colorPicker) : Tool(colorPicker, Tool::PatternPicker, "Color Picker (I)", ":/tools/color-picker.png", "I")
{
    this->setCursor(Picker);
}

void PatternPickerTool::mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(mouse.isButtonDown(Qt::LeftButton)){
        hud->fill(Qt::transparent);

        QPainter painter(hud);
        painter.setPen(QPen(Qt::NoPen));
        painter.setBrush(QBrush(AREA_BLUE));
        painter.setOpacity(.5);

        painter.drawRect(this->getRectPlusOne(mouse));
    }
}

void PatternPickerTool::createToolBar()
{
    QLabel *label = new QLabel("Save in: ", m_toolbar);
    m_saveIn = new QComboBox(m_toolbar);
    m_saveIn->addItem("Fill color");
    m_saveIn->addItem("Outline color");

    m_toolbar->addWidget(label);
    m_toolbar->addWidget(m_saveIn);
}

void PatternPickerTool::mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection)
{
    if(mouse.button() == Qt::LeftButton){
        hud->fill(Qt::transparent);

        ColorPicker::ColorType type = ColorPicker::Outline;
        if(m_saveIn->currentText() == "Fill color")
            type = ColorPicker::Fill;

        m_colorPicker->setPattern(type, PatternPickerTool::findPattern(*layer, this->getRectPlusOne(mouse)));
    }
}

QPixmap PatternPickerTool::findPattern(QImage image, QRect rect)
{
    rect.setBottom(std::min(rect.bottom(), image.height() -1));
    rect.setRight(std::min(rect.right(), image.width() -1));
    rect.setTop(std::max(rect.top(), 0));
    rect.setLeft(std::max(rect.left(), 0));

    // map of colors in the given rect used to access data much faster than pixel(x,y)
    QColor map[rect.width()][rect.height()];

    // sets of possible pattern sizes for each line and each column
    QSet<int> possibleSizesPerLine[rect.height()];
    QSet<int> possibleSizesPerColumn[rect.width()];

    // fill the map
    for(int i(rect.left()); i <= rect.right(); ++i)
        for(int j(rect.top()); j <= rect.bottom(); ++j)
            map[i - rect.left()][j - rect.top()] = QColor(image.pixel(i, j));

    // find possible pattern sizes for each line
    for(int line(0); line < rect.height(); ++line){

        // check each possible pattern size
        for(int size(1); size <= rect.width(); ++size){
            bool isPattern = true;

            // check if the line is a pattern
            for(int x(size); x < rect.width(); ++x){
                if(map[x][line] != map[x%size][line]){
                    isPattern = false;
                    break;
                }
            }

            if(isPattern)
                possibleSizesPerLine[line].insert(size);
        }
    }

    // same thing for columns
    for(int column(0); column < rect.width(); ++column){

        for(int size(1); size <= rect.height(); ++size){
            bool isPattern = true;

            for(int y(size); y < rect.height(); ++y){
                if(map[column][y] != map[column][y%size]){
                    isPattern = false;
                    break;
                }
            }

            if(isPattern)
                possibleSizesPerColumn[column].insert(size);
        }
    }

    // sets of possible pattern sizes, wich is the intersection of each sets
    QSet<int> possibleWidth = possibleSizesPerLine[0];
    QSet<int> possibleHeight = possibleSizesPerColumn[0];

    for(int line(0); line < rect.height(); ++line)
        possibleWidth = possibleWidth.intersect(possibleSizesPerLine[line]);
    for(int column(0); column < rect.width(); ++column)
        possibleHeight = possibleHeight.intersect(possibleSizesPerColumn[column]);

    // minimum possible size of the pattern
    int minWidth = rect.width();
    int minHeight = rect.height();

    QSetIterator<int> i(possibleWidth);
    while (i.hasNext())
        minWidth = std::min(minWidth, i.next());

    QSetIterator<int> j(possibleHeight);
    while (j.hasNext())
        minHeight = std::min(minHeight, j.next());

    QPixmap temp(minWidth, minHeight);
    temp.fill(Qt::transparent);
    QPixmap pattern = temp;

    QPainter painter(&temp);
    painter.drawImage(-rect.left(), -rect.top(), image);
    painter.end();

    painter.begin(&pattern);
    painter.drawPixmap(rect.left() % minWidth, rect.top() % minHeight, temp);
    painter.drawPixmap(rect.left() % minWidth - minWidth, rect.top() % minHeight, temp);
    painter.drawPixmap(rect.left() % minWidth, rect.top() % minHeight - minHeight, temp);
    painter.drawPixmap(rect.left() % minWidth - minWidth, rect.top() % minHeight - minHeight, temp);

    return pattern;
}
