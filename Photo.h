#ifndef PHOTO_H
#define PHOTO_H

#include <QtWidgets>

#define MIN_IMG_SIZE_W      100
#define MIN_IMG_SIZE_H      100

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

signals:
    void grabbed(QMouseEvent*, QPoint);

public slots:
    void resize(int value);
    void crop ();

private:
    QImage *loadedImage;
    QImage *printedImage;
    QPoint coord;
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
    QAction *act_crop;
    QAction *act_lower;
};

#endif // PHOTO_H
