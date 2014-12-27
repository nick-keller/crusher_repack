#ifndef IMAGECROPPER_H
#define IMAGECROPPER_H

#include <QtWidgets>
#include <cmath>
#include "../src/mousestate.h"
#include "../src/const.h"

#define SIZE 130

class ImageCropper : public QWidget
{
    Q_OBJECT
public:
    explicit ImageCropper(QWidget *parent = 0);

    void setImage(QImage image);

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

private:
    QImage getCroppedImage(bool grayscale = true);

signals:
    void imageCropped(QImage);
    void imageCropping(QImage);

public slots:
    void setCroppingCrop();
    void setCroppingFit();
    void setCroppingStretch();
    void setStretch(bool);

private:
    QImage m_image;
    QRect m_imageRect;
    QRect m_croppingZone;
    MouseState m_mouse;
    QPoint m_initPos;

    bool m_crop;
    bool m_fit;
    bool m_stretch;
};

#endif // IMAGECROPPER_H
