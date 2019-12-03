#ifndef SUBWIN_H
#define SUBWIN_H

#include <QtWidgets>
#include <QPicture>

#include "bubble.h"

#define MIN_IMG_SIZE_W      100
#define MIN_IMG_SIZE_H      100


class SubWin : public QWidget
{
    Q_OBJECT

public:
    SubWin(QWidget* parent = nullptr);
    ~SubWin();
    void mouseDoubleClickEvent(QMouseEvent*);

public slots:
    void loadImage();
    void save ();
    void reverseH();
    void reverseV();
    void moveElement(QMouseEvent*, QPoint);
    void addBubble();
    void updateEditingBubble();
    void resizeWidth(int value);
    void crop(QRect);
    void crop();

private:
    // Image
    QImage *loadedImage;
    QImage *m_img;
    QLabel *m_lab;
    bool reversedHorizontally;
    bool reversedVertically;
    bool croped;
    QRect cropDimensions;

    QPainter *m_painter;

    QPoint relativePos;

    QVector<Bubble*> bubbles;
    Bubble *movingBubble;
    Bubble *editingBubble;

    QSlider *hSlider;
    QSlider *vSlider;

    QSlider *sliderForImage;
};

#endif // SUBWIN_H
