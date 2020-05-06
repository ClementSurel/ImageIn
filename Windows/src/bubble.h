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
    QImage createFinalImage (int ratio);
    void setInactive();
    void mousePressEvent (QMouseEvent*);
    void mouseMoveEvent (QMouseEvent*);
    void mouseDoubleClickEvent (QMouseEvent*);
    void contextMenuEvent(QContextMenuEvent *event);
    void resizeEvent (QResizeEvent*);
    void resize(int ratio);

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
