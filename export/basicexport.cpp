#include "basicexport.h"

BasicExport::BasicExport(QImage image) :
    m_width(image.width()), m_height(image.height())
{
    this->convertImage(image);
}

void BasicExport::generate()
{
    BasicExportDialog *exportDialog = new BasicExportDialog();
    exportDialog->exec();
    delete exportDialog;

    this->findRects();
    this->findLines();

    QString code;
    QImage image(128, 64, QImage::Format_ARGB32);
    image.fill(Qt::white);
    QPainter painter(&image);

    for(int i(0); i < m_lines.size(); ++i){
        code += m_lines[i].getCode() + "<br>";
        m_lines[i].paint(&painter);
    }

    image.save("c.png");

    CodeDisplayDialog *codeDialog = new CodeDisplayDialog(code);
    codeDialog->exec();
    delete codeDialog;
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
            if(!isPxlOnOrRect(i, j))
                continue;

            // H -------------------------------------------------------------------------
            int hLength = 1;
            while(isPxlOnOrRect(i + hLength, j))
                hLength++;

            int above, below;
            for(int step(1); step <= hLength; ++step){
                above = 1;
                below = 1;

                if(!isHLineOnOrRect(i - step, j -1, step))
                    while(isHLineOnOrRect(i + above * step, j + above, step))
                        above++;
                above--;

                if(above == 1 && isPxlOnOrRect(i -1, j) && isPxlOnOrRect(i + step *2, j +1))
                    above = 0;

                if(above == 1 && step == 1  && isPxlOnOrRect(i, j -1) && isPxlOnOrRect(i +1, j +2))
                    above = 0;

                if(!isHLineOnOrRect(i - step, j +1, step))
                    while(isHLineOnOrRect(i + below * step, j - below, step))
                        below++;
                below--;

                if(below == 1 && isPxlOnOrRect(i -1, j) && isPxlOnOrRect(i + step *2, j -1))
                    below = 0;

                if(below == 1 && step == 1  && isPxlOnOrRect(i, j -1) && isPxlOnOrRect(i -1, j +2))
                    below = 0;

                if(above && lineHasAtLeastOnePxlNotRect(i, j, i + (above +1) * step -1, j + above))
                    lines.append(Line(i, j, i + (above +1) * step -1, j + above));

                if(below && lineHasAtLeastOnePxlNotRect(i, j, i + (below +1) * step -1, j - below))
                    lines.append(Line(i, j, i + (below +1) * step -1, j - below));
            }

            if(hLength != 1){
                if(!above && !below && !isPxlOnOrRect(i -1, j) && !isHLineOnOrRect(i - hLength, j -1, hLength) && !isHLineOnOrRect(i - hLength, j +1, hLength) && lineHasAtLeastOnePxlNotRect(i, j, i + hLength -1, j))
                    lines.append(Line(i, j, i + hLength -1, j));
            }

            // V -------------------------------------------------------------------------
            int vLength = 1;
            while(isPxlOnOrRect(i, j + vLength))
                vLength++;

            int right = 0, left = 0;
            for(int step(2); step <= vLength; ++step){
                right = 1;
                left = 1;

                if(!isVLineOnOrRect(i -1, j - step, step))
                    while(isVLineOnOrRect(i + right, j + right * step, step))
                        right++;
                right--;

                if(right == 1 && isPxlOnOrRect(i, j -1) && isPxlOnOrRect(i +1, j + step *2))
                    right = 0;

                if(!isVLineOnOrRect(i +1, j - step, step))
                    while(isVLineOnOrRect(i - left, j + left * step, step))
                        left++;
                left--;

                if(left == 1 && isPxlOnOrRect(i, j -1) && isPxlOnOrRect(i -1, j  + step *2))
                    left = 0;

                if(right && lineHasAtLeastOnePxlNotRect(i, j, i + right, j + (right +1) * step -1))
                    lines.append(Line(i, j, i + right, j + (right +1) * step -1));

                if(left && lineHasAtLeastOnePxlNotRect(i, j, i - left, j + (left +1) * step -1))
                    lines.append(Line(i, j, i - left, j + (left +1) * step -1));
            }

            if(vLength == 1 && (isPxlOnOrRect(i -1, j +1) || isPxlOnOrRect(i +1, j +1) || isPxlOnOrRect(i +1, j)|| isPxlOnOrRect(i -1, j)))
                continue;

            if(!right && !left && !isPxlOnOrRect(i, j -1) && !isVLineOnOrRect(i -1, j - vLength, vLength) && !isVLineOnOrRect(i +1, j - vLength, vLength) && lineHasAtLeastOnePxlNotRect(i, j, i, j + vLength -1))
                lines.append(Line(i, j, i, j + vLength -1));
        }
    }

    // filter
    IntMap pxlRedundancy = getIntMap();
    IntMap tempImg = getIntMap();

    for(int i(0); i < m_width; ++i)
        for(int j(0); j < m_height; ++j)
            if(m_rectArea[i][j]){
                tempImg[i][j] = 1;
                pxlRedundancy[i][j] = 1;
            }

    for(int l(0); l < lines.size(); ++l)
        addLineToRedundancyMap(pxlRedundancy, lines[l]);

    for(int l(0); l < lines.size(); ++l)
        if(lineHasExclusivePixel(pxlRedundancy, lines[l])){
            m_lines << Line(lines[l].from().x(), lines[l].from().y(), lines[l].to().x(), lines[l].to().y());


            for(LineIterator point(lines[l]); point.hasNext(); ++point)
                tempImg[point.x()][point.y()] = 1;

            lines.removeAt(l);
            l--;
        }

    qSort(lines.begin(), lines.end(), qGreater<Line>());

    for(int i(0); i < m_width; ++i)
        for(int j(0); j < m_height; ++j)
            if(m_rectArea[i][j])
                tempImg[i][j] = 1;

    for(int l(0); l < lines.size(); ++l){
        bool usefull = false;

        for(LineIterator point(lines[l]); point.hasNext(); ++point)
            if(tempImg[point.x()][point.y()] == 0){
                usefull = true;
                tempImg[point.x()][point.y()] = 1;
            }

        if(usefull);
            m_lines << Line(lines[l].from().x(), lines[l].from().y(), lines[l].to().x(), lines[l].to().y());
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
            heightMap[i][j] = isPxlOn(i, j) ? !j ? 1 : heightMap[i][j -1] +1 : 0;

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

bool BasicExport::isPxlOn(int x, int y)
{
    if(x < 0 || y < 0 || x >= m_width || y >= m_height)
        return false;

    return m_leftToDraw[x][y];
}

bool BasicExport::isPxlOnOrRect(int x, int y)
{
    if(x < 0 || y < 0 || x >= m_width || y >= m_height)
        return false;

    return m_leftToDraw[x][y] || m_rectArea[x][y];
}

bool BasicExport::isHLineOn(int x, int y, int length)
{
    for(int i(x); i < x + length; ++i)
        if(!isPxlOn(i, y))
            return false;

    return true;
}

bool BasicExport::isHLineOnOrRect(int x, int y, int length)
{
    for(int i(x); i < x + length; ++i)
        if(!isPxlOnOrRect(i, y))
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

bool BasicExport::isVLineOnOrRect(int x, int y, int length)
{
    for(int i(y); i < y + length; ++i)
        if(!isPxlOnOrRect(x, i))
            return false;

    return true;
}

bool BasicExport::lineHasAtLeastOnePxlNotRect(int x1, int y1, int x2, int y2)
{
    for(LineIterator point(Line(x1, y1, x2, y2)); point.hasNext(); ++point)
        if(isPxlOn(point.x(), point.y()))
            return true;

    return false;
}

void BasicExport::addLineToRedundancyMap(IntMap &map, Line line)
{
    for(LineIterator point(line); point.hasNext(); ++point){
        map[point.x()][point.y()] += 1;
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
