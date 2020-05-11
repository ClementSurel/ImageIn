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
        bool loadImage(int x, int y, int zoomRatio);
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
        void resizeWithGrip (QMouseEvent*, Grip::Corner);

    private:

        // Images
        QImage *loadedImage; // Original image, does not change over time
        QImage *printedImage; // Image showed on the screen

        int zoom;

        // Position and dimensions of the modified image excepting modifications with zoom
        int realX;
        int realY;
        int realWidth;
        int realHeight;

        // Properties
        bool isActive;

        // Transformations
        bool reversedHorizontally;
        bool reversedVertically;
        bool croped;
        QRect cropRect; // Part of the loaded image corresponding to the croped image before any other modification

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
