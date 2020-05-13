#ifndef BUBBLE_H
#define BUBBLE_H

#include <QPainter>
#include <QImage>
#include <QtWidgets>
#include <QMouseEvent>

#include "grip.h"


#define MIN_SIZE_W                  25
#define MIN_SIZE_H                  25

#define NATIVE_SIZE_W               80
#define NATIVE_SIZE_H               80

#define DEFAULT_FONT_POINTSIZE      72

#define MARGIN_BUBBLE               10


class Bubble : public QLabel
{
    Q_OBJECT

    public:

        Bubble(int x, int y, int ratio, QWidget *parent = nullptr);
        ~Bubble();

        // Create the image for saving
        QImage createFinalImage ();

        // mouse events
        void mousePressEvent (QMouseEvent*);
        //void mouseReleaseEvent(QMouseEvent*);
        void mouseMoveEvent (QMouseEvent*);
        void mouseDoubleClickEvent (QMouseEvent*);
        void contextMenuEvent(QContextMenuEvent *event);
        void resizeEvent (QResizeEvent*);
        void resizeWithZoom(int ratio);
        QImage resizeBubble (int newWidth, int newHeight);

    public slots:
        void resizeWithGrip (QMouseEvent *e, Grip::Corner corner);

    private:

        // Zoom
        int zoom;

        // Painter
        QPainter *painter;

        // Bubble image
        QImage *img;

        // Dimensions of the real bubble (when zoom is off)
        int realX;
        int realY;
        int realWidth;
        int realHeight;

        // Text
        QTextEdit *editingText;

        // grips
        Grip topLeftGrip;
        Grip topRightGrip;
        Grip downLeftGrip;
        Grip downRightGrip;

        // Position of the mouse when the user clicks
        QPoint clickPosistion;

        // ContextMenu
        QMenu *contextMenu;
        QAction *act_raise;
};

#endif // BUBBLE_H
