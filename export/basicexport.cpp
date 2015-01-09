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

    for(int i(0); i < m_drawables.size(); ++i)
        code += m_drawables[i]->getCode() + "<br>";

    CodeDisplayDialog *codeDialog = new CodeDisplayDialog(code);
    codeDialog->exec();
    delete codeDialog;
}

void BasicExport::convertImage(QImage image)
{
    QVector<bool> blankColumn(m_height, false);
    m_image = QVector< QVector<bool> >(m_width, blankColumn);

    for(int i(0); i < image.width(); ++i){
        for(int j(0); j < image.height(); ++j){
            if(QColor(image.pixel(i, j)) == BLACK)
                m_image[i][j] = true;
        }
    }
}

void BasicExport::findDots()
{

}

void BasicExport::findLines()
{
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

                if(above)
                    m_drawables.append(new Line(i, j, i + (above +1) * step -1, j + above));

                if(below)
                    m_drawables.append(new Line(i, j, i + (below +1) * step -1, j - below));
            }

            if(hLength != 1){
                if(!above && !below && !isPxlOn(i -1, j) && !isHLineOn(i - hLength, j -1, hLength) && !isHLineOn(i - hLength, j +1, hLength))
                    m_drawables.append(new Line(i, j, i + hLength -1, j));
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

                if(right)
                    m_drawables.append(new Line(i, j, i + right, j + (right +1) * step -1));

                if(left)
                    m_drawables.append(new Line(i, j, i - left, j + (left +1) * step -1));
            }

            if(vLength == 1 && (isPxlOn(i -1, j +1) || isPxlOn(i +1, j +1)))
                continue;

            if(!right && !left && !isPxlOn(i, j -1) && !isVLineOn(i -1, j - vLength, vLength) && !isVLineOn(i +1, j - vLength, vLength))
                m_drawables.append(new Line(i, j, i, j + vLength -1));
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

    // filter rects
    IntMap pxlRedundancy = getIntMap();
    for(int i(0); i < rects.size(); ++i){
        Rect r = rects[i];

        for(int i(r.x()); i < r.x() + r.w(); ++i)
            for(int j(r.y()); j < r.y() + r.h(); ++j)
                pxlRedundancy[i][j] += 1;
    }

    for(int i(0); i < m_width; ++i)
        for(int j(0); j < m_height; ++j)
            if(pxlRedundancy[i][j] == 1)
                for(int r(0); r < rects.size(); ++r)
                    if(rects[r].contains(i, j)){
                        m_drawables.append(new Rect(rects[r].x(), rects[r].y(), rects[r].w(), rects[r].h()));

                        // clear rect from image
                        for(int i(rects[r].x()); i < rects[r].x() + rects[r].w(); ++i)
                            for(int j(rects[r].y()); j < rects[r].y() + rects[r].h(); ++j)
                                m_image[i][j] = false;

                        rects.removeAt(r);
                        break;
                    }

}

bool BasicExport::isPxlOn(int x, int y)
{
    if(x < 0 || y < 0 || x >= m_width || y >= m_height)
        return false;

    return m_image[x][y];
}

bool BasicExport::isHLineOn(int x, int y, int length)
{
    for(int i(x); i < x + length; ++i)
        if(!isPxlOn(i, y))
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

IntMap BasicExport::getIntMap()
{
    QVector<int> blankColumn(m_height, 0);
    return IntMap(m_width, blankColumn);
}
