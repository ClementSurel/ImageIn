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

private:
    QImage *m_img;
    QImage *loadedImage;
    QPainter *m_painter;
    QLabel *m_lab;

    QPoint relativePos;

    QVector<Bubble*> bubbles;
    Bubble *movingBubble;
    Bubble *editingBubble;

    QSlider *hSlider;
    QSlider *vSlider;

    QSlider *sliderForImage;
};

#endif // SUBWIN_H
