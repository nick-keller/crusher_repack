#ifndef IMAGECROPPER_H
#define IMAGECROPPER_H

#include <QtWidgets>
#include <cmath>
#define SIZE 128

class ImageCropper : public QWidget
{
    Q_OBJECT
public:
    explicit ImageCropper(QWidget *parent = 0);

    void setImage(QImage image);

protected:
    void paintEvent(QPaintEvent*);

signals:

public slots:

private:
    QImage m_image;
    QRect m_imageRect;
    QRect m_croppingZone;
};

#endif // IMAGECROPPER_H
