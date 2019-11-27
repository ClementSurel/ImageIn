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
    void mouseDoubleClickEvent (QMouseEvent*);
    void focusOutEvent (QFocusEvent*);
    QImage createFinalImage ();

signals:
    void grabbed(QPoint);

private:
    QImage *img;
    QPainter *painter;

    QPoint relativePos;

    QTextEdit *text;

    QLabel *editingText;
};

#endif // BUBBLE_H
