#include "basicexport.h"

BasicExport::BasicExport(QImage image) :
    m_width(image.width()), m_height(image.height())
{
    this->convertImage(image);
}

void BasicExport::generate()
{
    BasicExportDialog *exportDialog = new BasicExportDialog();
    if(exportDialog->exec() == QDialog::Rejected)
        return;

    this->findRects();
    this->findLines();

    QString code;
    code += "ViewWindow 1,127,0,1,63,0,1,1,1<br>";

    code += this->getRectsCode(exportDialog);
    code += this->getLinesCode(exportDialog);
    code += this->getDotsCode(exportDialog);

    CodeDisplayDialog *codeDialog = new CodeDisplayDialog(code, m_rects.size(), m_lines.size(), m_dots.size());
    codeDialog->exec();

    delete codeDialog;
    delete exportDialog;
}

void BasicExport::convertImage(QImage image)
{
    m_leftToDraw = getBoolMap();
    m_rectArea = getBoolMap();
    m_originalImage = getBoolMap();

    for(int i(0); i < image.width(); ++i){
        for(int j(0); j < image.height(); ++j){
            if(QColor(image.pixel(i, j)) == BLACK){
                m_leftToDraw[i][j] = true;
                m_originalImage[i][j] = true;
            }
        }
    }
}

void BasicExport::findLines()
{
    QList<Line> lines;

    for(int j(0); j < m_height; ++j){
        for(int i(0); i < m_width; ++i){
            if(!isPxlOn(i, j))
                continue;

            // H -------------------------------------------------------------------------
            int hLength = 1;
            while(isPxlOn(i + hLength, j))
                hLength++;

            int above, below;
            for(int step(1); step <= hLength; ++step){
                above = 1;
                below = 1;

                if(!isHLineOn(i - step, j -1, step))
                    while(isHLineOn(i + above * step, j + above, step))
                        above++;
                above--;

                if(above == 1 && isPxlOn(i -1, j) && isPxlOn(i + step *2, j +1))
                    above = 0;

                if(above == 1 && step == 1  && isPxlOn(i, j -1) && isPxlOn(i +1, j +2))
                    above = 0;

                if(!isHLineOn(i - step, j +1, step))
                    while(isHLineOn(i + below * step, j - below, step))
                        below++;
                below--;

                if(below == 1 && isPxlOn(i -1, j) && isPxlOn(i + step *2, j -1))
                    below = 0;

                if(below == 1 && step == 1  && isPxlOn(i, j -1) && isPxlOn(i -1, j +2))
                    below = 0;

                if(above && lineHasAtLeastOnePxlLeftToDraw(i, j, i + (above +1) * step -1, j + above))
                    lines << Line(i, j, i + (above +1) * step -1, j + above);

                if(below && lineHasAtLeastOnePxlLeftToDraw(i, j, i + (below +1) * step -1, j - below))
                    lines << Line(i, j, i + (below +1) * step -1, j - below);
            }

            if(hLength != 1){
                if(!above && !below && !isPxlOn(i -1, j) && !isHLineOn(i - hLength, j -1, hLength) && !isHLineOn(i - hLength, j +1, hLength) && lineHasAtLeastOnePxlLeftToDraw(i, j, i + hLength -1, j))
                    lines << Line(i, j, i + hLength -1, j);
            }

            // V -------------------------------------------------------------------------
            int vLength = 1;
            while(isPxlOn(i, j + vLength))
                vLength++;

            int right = 0, left = 0;
            for(int step(2); step <= vLength; ++step){
                right = 1;
                left = 1;

                if(!isVLineOn(i -1, j - step, step))
                    while(isVLineOn(i + right, j + right * step, step))
                        right++;
                right--;

                if(right == 1 && isPxlOn(i, j -1) && isPxlOn(i +1, j + step *2))
                    right = 0;

                if(!isVLineOn(i +1, j - step, step))
                    while(isVLineOn(i - left, j + left * step, step))
                        left++;
                left--;

                if(left == 1 && isPxlOn(i, j -1) && isPxlOn(i -1, j  + step *2))
                    left = 0;

                if(right && lineHasAtLeastOnePxlLeftToDraw(i, j, i + right, j + (right +1) * step -1))
                    lines << Line(i, j, i + right, j + (right +1) * step -1);

                if(left && lineHasAtLeastOnePxlLeftToDraw(i, j, i - left, j + (left +1) * step -1))
                    lines << Line(i, j, i - left, j + (left +1) * step -1);
            }

            if(vLength == 1 && (isPxlOn(i -1, j +1) || isPxlOn(i +1, j +1) || isPxlOn(i +1, j)|| isPxlOn(i -1, j)))
                continue;

            if(!right && !left && !isPxlOn(i, j -1) && !isVLineOn(i -1, j - vLength, vLength) && !isVLineOn(i +1, j - vLength, vLength) && lineHasAtLeastOnePxlLeftToDraw(i, j, i, j + vLength -1))
                lines << Line(i, j, i, j + vLength -1);
        }
    }

    // filter
    IntMap pxlRedundancy = getIntMap();

    qSort(lines.begin(), lines.end(), qGreater<Line>());

    for(int i(0); i < lines.size(); ++i){
        int maxX = -1;
        int maxY = -1;
        int minX = SCREEN_WIDTH;
        int minY = SCREEN_HEIGHT;

        Line l = lines[i];

        for(LineIterator point(l); point.hasNext(); ++point){
            if(pxlRedundancy[point.x()][point.y()] == 0 && isPxlLeftToDraw(point.x(), point.y())){
                maxX = std::max(maxX, point.x());
                maxY = std::max(maxY, point.y());
                minX = std::min(minX, point.x());
                minY = std::min(minY, point.y());
            }
        }

        if(maxX == -1){
            lines.removeAt(i);
            i--;
        }
        else{
            l.constrain(minX, minY, maxX, maxY);

            addLineToRedundancyMap(pxlRedundancy, l);

            if(l.isDot())
                m_dots << Dot(l.from());
            else
                m_lines << l;
        }
    }
}

void BasicExport::findRects()
{
    IntMap heightMap = getIntMap();
    IntMap leftMap = getIntMap();
    IntMap rightMap = getIntMap();
    IntMap surfaceMap = getIntMap();
    QStack<int> left, right;
    QList<Rect> rects;

    // height map
    for(int j(0); j < m_height; ++j)
        for(int i(0); i < m_width; ++i)
            heightMap[i][j] = isPxlLeftToDraw(i, j) ? !j ? 1 : heightMap[i][j -1] +1 : 0;

    // left and right map
    for(int j(0); j < m_height; ++j){
        left.clear();
        for(int i(0); i < m_width; ++i){
            while(!left.empty() && heightMap[i][j] <= heightMap[left.top()][j])
                left.pop();

            if(left.empty())
                leftMap[i][j] = i;
            else
                leftMap[i][j] = i - left.top() -1;

            left.push(i);
        }

        right.clear();
        for(int i(m_width -1); i >= 0; --i){
            while(!right.empty() && heightMap[i][j] <= heightMap[right.top()][j])
                right.pop();

            if(right.empty())
                rightMap[i][j] = m_width -1 -i;
            else
                rightMap[i][j] = right.top() -i -1;

            right.push(i);
        }
    }

    // surface map
    for(int j(0); j < m_height; ++j)
        for(int i(0); i < m_width; ++i)
            surfaceMap[i][j] = heightMap[i][j] * (leftMap[i][j] + 1 + rightMap[i][j]);

    // get rects
    for(int j(0); j < m_height; ++j)
        for(int i(0); i < m_width; ++i){
            if(surfaceMap[i][j] && (j == m_height -1 || surfaceMap[i][j] > surfaceMap[i][j+1]) && heightMap[i][j] > 1 && leftMap[i][j] + rightMap[i][j]){
                bool keep = true;

                for(int right(1); right <= rightMap[i][j]; ++right)
                    if(heightMap[i][j] == heightMap[i+right][j]){
                        keep = false;
                        break;
                    }

                if(keep)
                    rects.append(Rect(i - leftMap[i][j], j - heightMap[i][j] +1, leftMap[i][j] + 1 + rightMap[i][j], heightMap[i][j]));
            }
        }

    // free
    heightMap.clear();
    leftMap.clear();
    rightMap.clear();
    surfaceMap.clear();
    left.clear();
    right.clear();

    // get only rects that have exlisive pixels
    IntMap pxlRedundancy = getIntMap();

    for(int i(0); i < rects.size(); ++i)
        addRectToRedundancyMap(pxlRedundancy, rects[i]);

    for(int i(0); i < m_width; ++i)
        for(int j(0); j < m_height; ++j)
            if(pxlRedundancy[i][j] == 1)
                for(int r(0); r < rects.size(); ++r)
                    if(rects[r].contains(i, j)){
                        m_rects << Rect(rects[r].x(), rects[r].y(), rects[r].w(), rects[r].h());

                        rects.removeAt(r);
                        r--;
                        break;
                    }


    // sort found rects, greter first
    qSort(m_rects.begin(), m_rects.end(), qGreater<Rect>());

    // filter unusefull rects due to overlap
    pxlRedundancy = getIntMap();
    for(int i(0); i < m_rects.size(); ++i){
        int maxX = 0;
        int maxY = 0;
        int minX = SCREEN_WIDTH;
        int minY = SCREEN_HEIGHT;

        Rect r = m_rects[i];

        for(int x(r.x()); x < r.x() + r.w(); ++x){
            for(int y(r.y()); y < r.y() + r.h(); ++y){
                if(pxlRedundancy[x][y] == 0){
                    maxX = std::max(maxX, x);
                    maxY = std::max(maxY, y);
                    minX = std::min(minX, x);
                    minY = std::min(minY, y);
                }

                pxlRedundancy[x][y] += 1;
            }
        }

        if(maxX - minX +1 <= 1 || maxY - minY +1 <= 1){
            addRectToRedundancyMap(pxlRedundancy, m_rects[i], -1);
            m_rects.removeAt(i);
            i--;
        }
        else{
            m_rects[i].setX(minX);
            m_rects[i].setY(minY);
            m_rects[i].setRight(maxX);
            m_rects[i].setBottom(maxY);
        }
    }

    // try to find others usefull rects
    qSort(rects.begin(), rects.end(), qGreater<Rect>());

    for(int i(0); i < rects.size(); ++i){
        int maxX = 0;
        int maxY = 0;
        int minX = SCREEN_WIDTH;
        int minY = SCREEN_HEIGHT;

        for(int x(rects[i].x()); x < rects[i].x() + rects[i].w(); ++x){
            for(int y(rects[i].y()); y < rects[i].y() + rects[i].h(); ++y){
                if(pxlRedundancy[x][y] == 0){
                    maxX = std::max(maxX, x);
                    maxY = std::max(maxY, y);
                    minX = std::min(minX, x);
                    minY = std::min(minY, y);
                }

                pxlRedundancy[x][y] += 1;
            }
        }

        if(maxX - minX +1 <= 1 || maxY - minY +1 <= 1){
            addRectToRedundancyMap(pxlRedundancy, rects[i], -1);
        }
        else{
            rects[i].setX(minX);
            rects[i].setY(minY);
            rects[i].setRight(maxX);
            rects[i].setBottom(maxY);

            m_rects << Rect(rects[i].x(), rects[i].y(), rects[i].w(), rects[i].h());
        }

        rects.removeAt(i);
        i--;
    }

    for(int i(0); i < m_rects.size(); ++i){
        for(int x(m_rects[i].x()); x < m_rects[i].x() + m_rects[i].w(); ++x){
            for(int y(m_rects[i].y()); y < m_rects[i].y() + m_rects[i].h(); ++y){
                m_rectArea[x][y] = true;
                m_leftToDraw[x][y] = false;
            }
        }
    }
}

bool BasicExport::isPxlLeftToDraw(int x, int y)
{
    if(x < 0 || y < 0 || x >= m_width || y >= m_height)
        return false;

    return m_leftToDraw[x][y];
}

bool BasicExport::isPxlOn(int x, int y)
{
    if(x < 0 || y < 0 || x >= m_width || y >= m_height)
        return false;

    return m_originalImage[x][y];
}

bool BasicExport::isHLineLeftToDraw(int x, int y, int length)
{
    for(int i(x); i < x + length; ++i)
        if(!isPxlLeftToDraw(i, y))
            return false;

    return true;
}

bool BasicExport::isHLineOn(int x, int y, int length)
{
    for(int i(x); i < x + length; ++i)
        if(!isPxlOn(i, y))
            return false;

    return true;
}

bool BasicExport::isVLineLeftToDraw(int x, int y, int length)
{
    for(int i(y); i < y + length; ++i)
        if(!isPxlLeftToDraw(x, i))
            return false;

    return true;
}

bool BasicExport::isVLineOn(int x, int y, int length)
{
    for(int i(y); i < y + length; ++i)
        if(!isPxlOn(x, i))
            return false;

    return true;
}

bool BasicExport::lineHasAtLeastOnePxlLeftToDraw(int x1, int y1, int x2, int y2)
{
    for(LineIterator point(Line(x1, y1, x2, y2)); point.hasNext(); ++point)
        if(isPxlLeftToDraw(point.x(), point.y()))
            return true;

    return false;
}

void BasicExport::addLineToRedundancyMap(IntMap &map, Line line, int value)
{
    for(LineIterator point(line); point.hasNext(); ++point){
        map[point.x()][point.y()] += value;
    }
}

bool BasicExport::lineHasExclusivePixel(IntMap &map, Line line)
{
    for(LineIterator point(line); point.hasNext(); ++point)
        if(map[point.x()][point.y()] == 1)
            return true;

    return false;
}

void BasicExport::addRectToRedundancyMap(IntMap &map, Rect r, int value)
{
    for(int i(r.x()); i < r.x() + r.w(); ++i)
        for(int j(r.y()); j < r.y() + r.h(); ++j)
            map[i][j] += value;
}

bool BasicExport::rectHasExclusivePixel(IntMap &map, Rect r)
{
    for(int i(r.x()); i < r.x() + r.w(); ++i)
        for(int j(r.y()); j < r.y() + r.h(); ++j)
            if(map[i][j] == 1)
                return true;

    return false;
}

QString BasicExport::getRectsCode(BasicExportDialog *dialog)
{
    QString code;

    if(dialog->rectsFunction() == BasicExportDialog::FLine){
        for(int i(0); i < m_rects.size(); ++i)
            code += m_rects[i].getFLineCode();
    }

    if(dialog->rectsFunction() == BasicExportDialog::ForLoop){
        for(int i(0); i < m_rects.size(); ++i)
            code += m_rects[i].getForLoopCode(dialog->rectsLoopVar()) + "<br>";
    }

    if(dialog->rectsFunction() == BasicExportDialog::DrawStat){

        if(m_rects.size() == 0)
            return "";

        code += "G-Connect<br>";


        if(dialog->drawStatStrategy() == BasicExportDialog::OutOfRangePoint){
            QList<QPoint> stops;
            int prevToMax = 1;

            for(int i(0); i < m_rects.size(); ++i){
                stops.append(m_rects[i].getDrawStatStops());

                if(i != m_rects.size() -1)
                    stops << QPoint(-1, SCREEN_HEIGHT);
            }

            for(int l(1); l < stops.size(); l += 254){
                int toMax(0);

                code += "{";
                for(int k(l -1); k < l + 254 && k < stops.size(); ++k){
                    code +=  QString::number(stops[k].x() +1) + (k == l + 253 || k == stops.size() -1? "" : ",");
                    toMax++;
                }
                code += "->List " + QString::number(dialog->rectsListX()) + "<br>";

                code += "{";
                for(int k(l -1); k < l + 254 && k < stops.size(); ++k)
                    code +=  QString::number(SCREEN_HEIGHT - stops[k].y()) + (k == l + 253 || k == stops.size() -1? "" : ",");
                code += "->List " + QString::number(dialog->rectsListY()) + "<br>";

                if(prevToMax != toMax)
                    code += QString::number(toMax) + "->Tomax<br>";
                prevToMax = toMax;

                code += "Graph(X,Y)=(List " + QString::number(dialog->rectsListX()) + "[T,List " + QString::number(dialog->rectsListY()) + "[T<br>";
            }
        }
        else{
            int prevToMax = 1;

            for(int i(0); i < m_rects.size(); ++i){
                int toMax(0);
                QList<QPoint> stops = m_rects[i].getDrawStatStops();

                code += "{";
                for(int k(0); k < stops.size(); ++k){
                    code +=  QString::number(stops[k].x() +1) + (k == stops.size() -1? "" : ",");
                    toMax++;
                }
                code += "->List " + QString::number(dialog->rectsListX()) + "<br>";

                code += "{";
                for(int k(0); k < stops.size(); ++k)
                    code +=  QString::number(SCREEN_HEIGHT - stops[k].y()) + (k == stops.size() -1? "" : ",");
                code += "->List " + QString::number(dialog->rectsListY()) + "<br>";

                if(prevToMax != toMax)
                    code += QString::number(toMax) + "->Tomax<br>";
                prevToMax = toMax;

                code += "Graph(X,Y)=(List " + QString::number(dialog->rectsListX()) + "[T,List " + QString::number(dialog->rectsListY()) + "[T<br>";
            }
        }
    }

    return code;
}

QString BasicExport::getLinesCode(BasicExportDialog *dialog)
{
    QString code;

    if(dialog->linesFunction() == BasicExportDialog::FLine){
        for(int i(0); i < m_lines.size(); ++i)
            code += m_lines[i].getFLineCode() + "<br>";
    }

    if(dialog->linesFunction() == BasicExportDialog::DrawStat){
        if(dialog->drawStatStrategy() == BasicExportDialog::OutOfRangePoint){
            QList<QPoint> stops;
            int prevToMax = 1;

            for(int i(0); i < m_lines.size(); ++i){
                stops << m_lines[i].from() << m_lines[i].to();

                if(i != m_lines.size() -1)
                    stops << QPoint(-1, SCREEN_HEIGHT);
            }

            for(int l(1); l < stops.size(); l += 254){
                int toMax(0);

                code += "{";
                for(int k(l -1); k < l + 254 && k < stops.size(); ++k){
                    code +=  QString::number(stops[k].x() +1) + (k == l + 253 || k == stops.size() -1? "" : ",");
                    toMax++;
                }
                code += "->List " + QString::number(dialog->linesListX()) + "<br>";

                code += "{";
                for(int k(l -1); k < l + 254 && k < stops.size(); ++k)
                    code +=  QString::number(SCREEN_HEIGHT - stops[k].y()) + (k == l + 253 || k == stops.size() -1? "" : ",");
                code += "->List " + QString::number(dialog->linesListY()) + "<br>";

                if(prevToMax != toMax)
                    code += QString::number(toMax) + "->Tomax<br>";
                prevToMax = toMax;

                code += "Graph(X,Y)=(List " + QString::number(dialog->linesListX()) + "[T,List " + QString::number(dialog->linesListY()) + "[T<br>";
            }
        }
        else{
            code += "2->Tomax<br>";

            for(int i(0); i < m_lines.size(); ++i){
                code += "{" + QString::number(m_lines[i].from().x() +1) + "," + QString::number(m_lines[i].to().x() +1);
                code += "->List " + QString::number(dialog->linesListX()) + "<br>";

                code += "{" + QString::number(SCREEN_HEIGHT - m_lines[i].from().y()) + "," + QString::number(SCREEN_HEIGHT - m_lines[i].to().y());
                code += "->List " + QString::number(dialog->linesListY()) + "<br>";

                code += "Graph(X,Y)=(List " + QString::number(dialog->linesListX()) + "[T,List " + QString::number(dialog->linesListY()) + "[T<br>";
            }
        }
    }

    return code;
}

QString BasicExport::getDotsCode(BasicExportDialog *dialog)
{
    QString code;

    if(dialog->dotsFunction() == BasicExportDialog::PlotOn){
        for(int i(0); i < m_dots.size(); ++i)
            code += m_dots[i].getPlotOnCode() + "<br>";
    }

    if(dialog->dotsFunction() == BasicExportDialog::PxlOn){
        for(int i(0); i < m_dots.size(); ++i)
            code += m_dots[i].getPxlOnCode() + "<br>";
    }

    if(dialog->dotsFunction() == BasicExportDialog::FLine){
        for(int i(0); i < m_dots.size(); ++i)
            code += m_dots[i].getFLineCode() + "<br>";
    }

    if(dialog->dotsFunction() == BasicExportDialog::DrawStat){

        if(m_dots.size() == 0)
            return "";

        code += "S-Gph1 DrawOn,Scatter,List " + QString::number(dialog->dotsListX()) + ",List " + QString::number(dialog->dotsListY()) + ",1,Dot<br>";

        for(int l(0); l < m_dots.size(); l += 255){
            code += "{";
            for(int k(l); k < l + 255 && k < m_dots.size(); ++k)
                code +=  QString::number(m_dots[k].x() +1) + (k == l + 254 || k == m_dots.size() -1? "" : ",");
            code += "->List " + QString::number(dialog->dotsListX()) + "<br>";

            code += "{";
            for(int k(l); k < l + 255 && k < m_dots.size(); ++k)
                code +=  QString::number(SCREEN_HEIGHT - m_dots[k].y()) + (k == l + 254 || k == m_dots.size() -1? "" : ",");
            code += "->List " + QString::number(dialog->dotsListY()) + "<br>";
            code += "DrawStat<br>";
        }
    }

    return code;
}

IntMap BasicExport::getIntMap()
{
    QVector<int> blankColumn(m_height, 0);
    return IntMap(m_width, blankColumn);
}

BoolMap BasicExport::getBoolMap()
{
    QVector<bool> blankColumn(m_height, false);
    return BoolMap(m_width, blankColumn);
}
