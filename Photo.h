#ifndef PHOTO_H
#define PHOTO_H

#include <QtWidgets>

#include "grip.h"

#define MIN_IMG_SIZE_W      20
#define MIN_IMG_SIZE_H      20

class Photo : public QLabel
{
    Q_OBJECT

public:
    Photo(QWidget*);
    ~Photo();
    void loadImage();
    void reverseH();
    void reverseV();
    QImage finalImage();
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void contextMenuEvent(QContextMenuEvent*);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent*);
    void resizeImage(int newWidth, int newHeight);

signals:
    void grabbed(QMouseEvent*, QPoint);

public slots:
    void crop ();
    void resizeFilter (QMouseEvent*, Grip::Corner);

private:
    QImage *loadedImage;
    QImage *printedImage;
    QPoint coord;
    qreal imageRatio;
    bool reversedHorizontally;
    bool reversedVertically;
    bool croped;
    QRect cropRect;
    bool locked;

    QPoint relativePos;

    // mouse button
    Qt::MouseButton mouseButton;
    bool selecting;
    QRubberBand selection;
    QPoint clickPoint;

    // Context menu
    QMenu *contextMenu;
    QAction *act_crop;
    //QAction *act_lower;

    // grips
    Grip topLeftGrip;
    Grip topRightGrip;
    Grip bottomLeftGrip;
    Grip bottomRightGrip;
};

#endif // PHOTO_H
