#ifndef BUBBLE_H
#define BUBBLE_H

#include <QPainter>
#include <QImage>
#include <QtWidgets>
#include <QMouseEvent>


#define MIN_SIZE_W      80
#define MIN_SIZE_H      80


class Bubble : public QLabel
{
    Q_OBJECT

public:
    Bubble(QWidget *parent = nullptr);
    ~Bubble();
    void mousePressEvent (QMouseEvent*);
    void mouseMoveEvent (QMouseEvent*);
    void mouseDoubleClickEvent (QMouseEvent*);
    QImage createFinalImage ();

signals:
    void grabbed(QPoint);

public slots:
    void resizeWidth(int value);
    void resizeHeight(int value);

private:
    QImage *img;
    QPainter *painter;

    QPoint relativePos;

    QTextEdit *text;

    QLabel *editingText;
};

#endif // BUBBLE_H
