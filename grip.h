#ifndef GRIP_H
#define GRIP_H

#include <QtWidgets>

class Grip : public QSizeGrip
{   
    Q_OBJECT

public:
    // Definition of Corner enumeration
    typedef enum Corner
    {topLeft, topRight, bottomLeft, bottomRight} Corner;

    Grip(QWidget*, Corner);
    void mouseMoveEvent(QMouseEvent*);

signals:
    void grabbed(QMouseEvent*, Grip::Corner);

private:
    Corner corner;
};

#endif // GRIP_H
