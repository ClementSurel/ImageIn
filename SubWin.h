#ifndef SUBWIN_H
#define SUBWIN_H

#include <QtWidgets>
#include <QPicture>

#include "bubble.h"

class SubWin : public QWidget
{
    Q_OBJECT

public:
    SubWin();
    ~SubWin();

public slots:
    void save ();
    void reverseH();
    void reverseV();
    void moveElement(QPoint);

private:
    QImage *m_img;
    QPainter *m_painter;
    QLabel *m_lab;

    QPoint relativePos;

public:
    Bubble *bubble;
};

#endif // SUBWIN_H
