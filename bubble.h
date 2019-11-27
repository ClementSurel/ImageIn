#ifndef BUBBLE_H
#define BUBBLE_H

#include <QPainter>
#include <QImage>
#include <QLabel>
#include <QMouseEvent>
#include <QTextEdit>

class Bubble : public QLabel
{
    Q_OBJECT

public:
    Bubble(QWidget *parent);
    ~Bubble();
    void mousePressEvent (QMouseEvent*);
    void mouseMoveEvent (QMouseEvent*);

signals:
    void grabbed(QPoint);

private:
    QImage *img;
    QPainter *painter;

    QPoint relativePos;

    QTextEdit *text;
};

#endif // BUBBLE_H
