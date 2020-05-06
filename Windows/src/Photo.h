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
    bool loadImage(int zoomRatio);
    QImage finalImage(int zoomRatio);
    void mousePressEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent*);
    void contextMenuEvent(QContextMenuEvent*);
    QImage resizeImage(int newWidth, int newHeight);
    void resizeEvent(QResizeEvent*);
    void resizeWithZoom (int ratio);

signals:
    void grabbed(QMouseEvent*, QPoint);
    void activated();
    void isSelecting(bool);

public slots:
    void reverseH();
    void reverseV();
    void crop ();
    void resizeEverything (QMouseEvent*, Grip::Corner);

private:
    // Properties
    bool isActive;

    // Images
    QImage *loadedImage;
    QImage *printedImage;

    // Transformations
    bool reversedHorizontally;
    bool reversedVertically;
    bool croped;
    QRect cropRect;

    QPoint relativePos;

    // mouse button
    Qt::MouseButton mouseButton;
    bool selecting;
    QRubberBand selection;
    QPoint clickPoint;

    // Context menu
    QMenu *contextMenu;
    QAction *act_reverseH;
    QAction *act_reverseV;
    QAction *act_crop;
    QAction *act_raise;
    QAction *act_suppr;
    //QAction *act_lower;

    // grips
    Grip topLeftGrip;
    Grip topRightGrip;
    Grip bottomLeftGrip;
    Grip bottomRightGrip;
};

#endif // PHOTO_H
