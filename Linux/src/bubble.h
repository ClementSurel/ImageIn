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
    void setInactive();
    void resizeEvent (QResizeEvent*);
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    void editing();

private:
    // Bubble image
    QImage *img;
    QPainter *painter;
    // Position
    QPoint relativePos;
    // Text
    QTextEdit *editingText;
    QLabel *printedText;
    // ContextMenu
    QMenu *contextMenu;
    QAction *act_raise;
    // grips
    QSizeGrip topLeftGrip;
    QSizeGrip topRightGrip;
    QSizeGrip downLeftGrip;
    QSizeGrip downRightGrip;
};

#endif // BUBBLE_H