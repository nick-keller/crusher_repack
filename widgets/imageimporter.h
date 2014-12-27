#ifndef IMAGEIMPORTER_H
#define IMAGEIMPORTER_H

#include <QtWidgets>
#include <cmath>
#include "../src/const.h"

class ImageImporter : public QWidget
{
    Q_OBJECT
public:
    explicit ImageImporter(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);

private:
    void createBrushes();

signals:

public slots:
    void setImage(QImage);
    void setPreview(QImage);
    void setDithering(bool);
    void setDitheringLevel(int);

private:
    QImage m_original;
    QImage m_result;
    bool m_showOriginal;
    bool m_dithering;
    int m_ditheringLevel;
    QList<QBrush> m_brushes;
};

#endif // IMAGEIMPORTER_H
