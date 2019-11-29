#ifndef SUBWIN_H
#define SUBWIN_H

#include <QtWidgets>
#include <QPicture>

#include "bubble.h"


class SubWin : public QWidget
{
    Q_OBJECT

public:
    SubWin(QWidget* parent = nullptr);
    ~SubWin();

public slots:
    void loadImage();
    void save ();
    void reverseH();
    void reverseV();
    void moveElement(QPoint);
    void addBubble();

private:
    QImage *m_img;
    QPainter *m_painter;
    QLabel *m_lab;

    QPoint relativePos;

    QVector<Bubble*> bubbles;
    Bubble *activeBubble;

    QSlider *hSlider;
    QSlider *vSlider;
};

#endif // SUBWIN_H
