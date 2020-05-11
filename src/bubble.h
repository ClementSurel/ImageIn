#ifndef BUBBLE_H
#define BUBBLE_H

#include <QPainter>
#include <QImage>
#include <QtWidgets>
#include <QMouseEvent>


#define MIN_SIZE_W      1
#define MIN_SIZE_H      1

#define NATIVE_SIZE_W      80
#define NATIVE_SIZE_H      80

#define DEFAULT_FONT_POINTSIZE  72


class Bubble : public QLabel
{
    Q_OBJECT

    public:

        Bubble(int x, int y, int ratio, QWidget *parent = nullptr);
        ~Bubble();

        QImage createFinalImage (int ratio);

        // mouse events
        void mousePressEvent (QMouseEvent*);
        //void mouseReleaseEvent(QMouseEvent*);
        void mouseMoveEvent (QMouseEvent*);
        void mouseDoubleClickEvent (QMouseEvent*);
        void contextMenuEvent(QContextMenuEvent *event);
        void resizeEvent (QResizeEvent*);
        void resize(int ratio);

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
        QSizeGrip topLeftGrip;
        QSizeGrip topRightGrip;
        QSizeGrip downLeftGrip;
        QSizeGrip downRightGrip;

        // Position of the mouse when the user clicks
        QPoint clickPosistion;

        // ContextMenu
        QMenu *contextMenu;
        QAction *act_raise;
};

#endif // BUBBLE_H
