#include "tool.h"

Tool::Tool(ColorPicker *colorPicker, Name toolName, QString name, QString icon, QString shortcut) :
    m_toolName(toolName), m_name(name), m_icon(icon), m_shortcut(shortcut), m_toolbar(0),
    m_colorPicker(colorPicker),
    m_joinType(0), m_brushSize(0), m_outlineType(0), m_capType(0)
{
    m_cursors.insert(Bucket, new QCursor(QPixmap(":/cursors/bucket.png"), 1, 1));
    m_cursors.insert(Picker, new QCursor(QPixmap(":/cursors/color-picker.png"), 2, 15));
    m_cursors.insert(Cross, new QCursor(QPixmap(":/cursors/cross.png")));
    m_cursors.insert(CrossPlus, new QCursor(QPixmap(":/cursors/cross-plus.png"), 9, 9));
    m_cursors.insert(CrossMinus, new QCursor(QPixmap(":/cursors/cross-minus.png"), 9, 9));
    m_cursors.insert(CrossAnd, new QCursor(QPixmap(":/cursors/cross-and.png"), 9, 9));
    m_cursors.insert(Duplicate, new QCursor(QPixmap(":/cursors/duplicate.png"), 1, 1));
    m_cursors.insert(Hand, new QCursor(QPixmap(":/cursors/hand.png"), 7, 1));
    m_cursors.insert(MoveCursor, new QCursor(QPixmap(":/cursors/move.png"), 1, 1));
    m_cursors.insert(MoveCut, new QCursor(QPixmap(":/cursors/move-cut.png"), 1, 1));
    m_cursors.insert(Selection, new QCursor(QPixmap(":/cursors/select.png"), 1, 1));
    m_cursors.insert(SelectLayer, new QCursor(QPixmap(":/cursors/select-layer.png"), 7, 1));
    m_cursors.insert(Wand, new QCursor(QPixmap(":/cursors/wand.png"), 7, 6));

    this->setCursor(Cross);
}

QAction *Tool::getAction(QObject * parent)
{
    QAction *action = new QAction(m_name, parent);
    action->setShortcut(QKeySequence(m_shortcut));
    action->setIcon(QIcon(m_icon));
    action->setCheckable(true);
    action->setProperty("tool", m_toolName);

    return action;
}

QToolBar *Tool::getToolBar(QWidget *parent)
{
    if(m_toolbar != 0)
        return m_toolbar;

    m_toolbar = new QToolBar(parent);
    m_toolbar->setAllowedAreas(Qt::TopToolBarArea);
    m_toolbar->setMovable(false);
    m_toolbar->setFloatable(false);
    m_toolbar->setFixedHeight(40);

    QLabel* icon = new QLabel(m_toolbar);
    icon->setPixmap(QPixmap(m_icon));
    icon->setMargin(8);
    m_toolbar->addWidget(icon);
    m_toolbar->addSeparator();
    this->createSpacer();

    this->createToolBar();
    return m_toolbar;
}

QCursor *Tool::getCursor()
{
    return m_cursor;
}

void Tool::keyPressEvent(Qt::Key key)
{
    m_keysDown.insert(key);
}

void Tool::keyReleaseEvent(Qt::Key key)
{
    m_keysDown.remove(key);
}

QComboBox *Tool::createAngleSelector()
{
    QLabel *label = new QLabel("Corners: ", m_toolbar);
    QComboBox *selector = new QComboBox(m_toolbar);
    selector->addItem(QIcon(":/icons/miter-join.png"), "Miter");
    selector->addItem(QIcon(":/icons/bevel-join.png"), "Bevel");
    selector->addItem(QIcon(":/icons/round-join.png"), "Round");

    m_toolbar->addWidget(label);
    m_toolbar->addWidget(selector);
    this->createSpacer();

    m_joinType = selector;

    return selector;
}

QComboBox *Tool::createOutlineSelector()
{
    QComboBox *selector = new QComboBox(m_toolbar);
    selector->addItem(QIcon(":/icons/inside.png"), "Inside");
    selector->addItem(QIcon(":/icons/middle.png"), "Middle");
    selector->addItem(QIcon(":/icons/outside.png"), "Outside");

    m_toolbar->addWidget(selector);
    this->createSpacer();

    m_outlineType = selector;

    return selector;
}

QComboBox *Tool::createCapSelector()
{
    QLabel *label = new QLabel("Cap style: ", m_toolbar);
    QComboBox *selector = new QComboBox(m_toolbar);
    selector->addItem(QIcon(":/icons/flat-cap.png"), "Flat");
    selector->addItem(QIcon(":/icons/round-cap.png"), "Round");
    selector->addItem(QIcon(":/icons/square-cap.png"), "Square");

    m_toolbar->addWidget(label);
    m_toolbar->addWidget(selector);
    this->createSpacer();

    m_capType = selector;

    return selector;
}

QSpinBox *Tool::createBrushSizeSelector()
{
    QSpinBox *selector = new QSpinBox(m_toolbar);
    selector->setMinimum(0);
    selector->setMaximum(64);
    selector->setSuffix(" px");
    selector->setValue(1);

    MagicLabel *label = new MagicLabel("Outline: ", selector, m_toolbar);

    m_toolbar->addWidget(label);
    m_toolbar->addWidget(selector);
    this->createSpacer();

    m_brushSize = selector;
    return selector;
}

void Tool::createSpacer()
{
    QWidget *spacer = new QWidget;
    spacer->setFixedWidth(5);
    m_toolbar->addWidget(spacer);
}

QPainter *Tool::getPainter(QPaintDevice *device)
{
    QPainter *painter = new QPainter(device);

    QPen pen;
    pen.setBrush(this->getOutlineBrush());

    if(m_brushSize != 0){
        pen.setWidth(m_brushSize->value());
        if(m_brushSize->value() == 0)
            pen = QPen(Qt::NoPen);
    }

    if(m_joinType != 0){
        if("Miter" == m_joinType->currentText())
            pen.setJoinStyle(Qt::MiterJoin);
        else if("Bevel" == m_joinType->currentText())
            pen.setJoinStyle(Qt::BevelJoin);
        else
            pen.setJoinStyle(Qt::RoundJoin);
    }

    if(m_capType != 0){
        if("Flat" == m_capType->currentText())
            pen.setCapStyle(Qt::FlatCap);
        else if("Round" == m_capType->currentText())
            pen.setCapStyle(Qt::RoundCap);
        else
            pen.setCapStyle(Qt::SquareCap);
    }

    QBrush brush = this->getFillBrush();

    painter->setPen(pen);
    painter->setBrush(brush);

    return painter;
}

void Tool::drawPerfectLine(QPainter *painter, int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1, dy = y2 - y1;

    painter->drawPoint(x1, y1);
    painter->drawPoint(x2, y2);

    if(dx != 0){
        if(dy != 0){
            if(dx < 0){ // swap point
                int t1 = x1, t2 = y1;
                x1 = x2;
                x2 = t1;
                y1 = y2;
                y2 = t2;

                dx = x2 - x1;
                dy = y2 - y1;
            }

            if(dx >= std::abs(dy)){
                int e = dx;
                dx *= 2;
                dy *= 2;

                while(true){
                    painter->drawPoint(x1++, y1);
                    if(x1 == x2) break;
                    if((e += (dy > 0 ? -dy : dy)) < 0){
                        y1 += (dy > 0 ? 1 : -1);
                        e += dx;
                    }
                }
            } else {
                int e = dy;
                dx *= 2;
                dy *= 2;

                while(true){
                    painter->drawPoint(x1, y1);
                    y1 += dy > 0 ? 1 : -1;
                    if(y1 == y2) break;
                    e += dy > 0 ? -dx : dx;
                    if(e < 0 && dy > 0 || e > 0 && dy < 0){
                        x1++;
                        e += dy;
                    }
                }
            }
        } else { // dy == 0
            int xMin = x1 < x2 ? x1 : x2; // Vertical line
            int xMax = x1 + x2 - xMin;

            for(int i(xMin); i <= xMax; ++i)
                painter->drawPoint(i, y1);
        }

    } else { // dx == 0
        int yMin = y1 < y2 ? y1 : y2; // Horizontal line
        int yMax = y1 + y2 - yMin;

        for(int i(yMin); i <= yMax; ++i)
            painter->drawPoint(x1, i);
    }
}

void Tool::drawPerfectLine(QPainter *painter, QPoint p1, QPoint p2)
{
    Tool::drawPerfectLine(painter, p1.x(), p1.y(), p2.x(), p2.y());
}

void Tool::drawPerfectEllipse(QPainter *painter, int left, int top, int width, int height, bool fill)
{
    int
        rx = width / 2,
        ry = height / 2,
        cx = left + rx,
        cy = top + ry,
        twoASquare = 2 * rx * rx,
        twoBSquare = 2 * ry * ry,

        x = rx,
        y = 0,
        xChange = ry * ry * (1 - 2 * rx),
        yChange = rx * rx,
        e = 0,
        stoppingX = twoBSquare * rx,
        stoppingY = 0,

        addX = width - rx *2,
        addY = height - ry *2
        ;

    if(rx * ry == 0) return;

    while(stoppingX >= stoppingY){
        painter->drawPoint(cx + x + addX, cy + y + addY);
        painter->drawPoint(cx + x + addX, cy - y);
        painter->drawPoint(cx - x, cy + y + addY);
        painter->drawPoint(cx - x, cy - y);

        if(fill){
            Tool::drawPerfectLine(painter, cx + x + addX, cy + y + addY, cx - x, cy + y + addY);
            Tool::drawPerfectLine(painter, cx + x + addX, cy - y, cx - x, cy - y);
        }

        y++;
        stoppingY += twoASquare;
        e += yChange;
        yChange += twoASquare;

        if(2* e + xChange > 0){
            x--;
            stoppingX -= twoBSquare;
            e += xChange;
            xChange += twoBSquare;
        }
    }

    x = 0;
    y = ry;
    xChange = ry * ry;
    yChange = rx * rx * (1- 2 * ry);
    e = 0;
    stoppingX = 0;
    stoppingY = twoASquare * ry;

    while(stoppingX <= stoppingY){
        painter->drawPoint(cx + x + addX, cy + y + addY);
        painter->drawPoint(cx + x + addX, cy - y);
        painter->drawPoint(cx - x, cy + y + addY);
        painter->drawPoint(cx - x, cy - y);

        if(fill){
            Tool::drawPerfectLine(painter, cx + x + addX, cy + y + addY, cx + x + addX, cy - y);
            Tool::drawPerfectLine(painter, cx - x, cy + y + addY, cx - x, cy - y);
        }

        x++;
        stoppingX += twoBSquare;
        e += xChange;
        xChange += twoBSquare;

        if(2* e + yChange >= 0){
            y--;
            stoppingY -= twoASquare;
            e += yChange;
            yChange += twoASquare;
        }
    }
}

void Tool::drawPerfectEllipse(QPainter *painter, QRect ellipse, bool fill)
{
    Tool::drawPerfectEllipse(painter, ellipse.x(), ellipse.y(), ellipse.width(), ellipse.height(), fill);
}

void Tool::drawInSelection(QImage *target, QImage *source, QImage *selection, bool *useSelection)
{
    if(*useSelection){
        QPainter sourcePainter(source);
        sourcePainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        sourcePainter.drawImage(0, 0, *selection);
    }

    QPainter targetPainter(target);
    targetPainter.drawImage(0, 0, *source);
}

QBrush Tool::getFillBrush()
{
    return m_colorPicker->getFillBrush();
}

QPen Tool::getFillPen()
{
    return QPen(this->getFillBrush(), 1);
}

QBrush Tool::getOutlineBrush()
{
    return m_colorPicker->getOutlineBrush();
}

QPen Tool::getOutlinePen()
{
    return QPen(this->getOutlineBrush(), 1);
}

QRect Tool::getRectPlusOne(QRect rect)
{
    rect.setSize(QSize(rect.width() +1, rect.height() +1));
    return rect;
}

QRect Tool::getRectPlusOne(MouseState mouse)
{
    return this->getRectPlusOne(mouse.getClickedRect());
}

void Tool::constrainRect(MouseState mouse, QRect *rect)
{

    if(m_keysDown.contains(Qt::Key_Alt)){
        QSize newSize(rect->width() *2 -1, rect->height() *2 -1);

        if(m_keysDown.contains(Qt::Key_Shift)){
            newSize.setWidth(std::min(newSize.width(), newSize.height()));
            newSize.setHeight(newSize.width());
        }

        rect->setX(mouse.clickedX() - newSize.width() /2);
        rect->setY(mouse.clickedY() - newSize.height() /2);
        rect->setSize(newSize);
    }

    else if(m_keysDown.contains(Qt::Key_Shift)){

        int newSize = std::min(rect->width(), rect->height());

        if(rect->width() < rect->height()){
            if(mouse.y() < mouse.clickedY()){
                rect->setY(mouse.clickedY() - newSize +1);
            }
        }
        else{
            if(mouse.x() < mouse.clickedX()){
                rect->setX(mouse.clickedX() - newSize +1);
            }
        }

        rect->setHeight(newSize);
        rect->setWidth(newSize);
    }
}

void Tool::setCursor(Tool::Cursor cursor)
{
    if(m_cursor == m_cursors[cursor])
        return;

    m_cursor = m_cursors[cursor];
    emit cursorChanged(m_cursor);
}
