#ifndef BUBBLE_H
#define BUBBLE_H

#include <QPainter>
#include <QImage>
#include <QtWidgets>
#include <QMouseEvent>


#define MIN_SIZE_W      80
#define MIN_SIZE_H      80

#define DEFAULT_FONT_POINTSIZE  72


class Bubble : public QLabel
{
    Q_OBJECT

    public:

        Bubble(int ratio, QWidget *parent = nullptr);
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

        // Painter
        QPainter *painter;

        // Bubble image
        QImage *img;

        // Text
        QTextEdit *editingText;

        // Position of the mouse when the user clicks
        QPoint clickPosistion;

        // grips
        QSizeGrip topLeftGrip;
        QSizeGrip topRightGrip;
        QSizeGrip downLeftGrip;
        QSizeGrip downRightGrip;

        // ContextMenu
        QMenu *contextMenu;
        QAction *act_raise;
};

#endif // BUBBLE_H
