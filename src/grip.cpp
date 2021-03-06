#include <QPainter>

#include "grip.h"

Grip::Grip(QWidget *parent, Grip::Corner c) : QSizeGrip(parent)
{
    corner = c;
}

void Grip::mouseMoveEvent(QMouseEvent *e)
{
    emit grabbed(e, corner);
}

void Grip::paintEvent (QPaintEvent *e)
{
    QSizeGrip::paintEvent(e);
}
