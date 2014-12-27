#ifndef IMAGEIMPORTER_H
#define IMAGEIMPORTER_H

#include <QtWidgets>
#include "../src/const.h"

class ImageImporter : public QWidget
{
    Q_OBJECT
public:
    explicit ImageImporter(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:
    void setImage(QImage);
    void setPreview(QImage);

private:
    QImage m_original;
    QImage m_result;
    bool m_showOriginal;

};

#endif // IMAGEIMPORTER_H
