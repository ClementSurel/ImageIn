#ifndef SUBWIN_H
#define SUBWIN_H

#include <QtWidgets>
#include <QtWidgets>
#include <QPicture>

#include "Photo.h"
#include "bubble.h"


#define PAGE_W          2480
#define PAGE_H          3508


class SubWin : public QWidget
{
    Q_OBJECT

    public:
        SubWin(QScrollArea* givenScroll, QWidget* parent = nullptr);
        ~SubWin();
        void resizePage(bool zoomIn);
        void contextMenuEvent(QContextMenuEvent *e);

    signals:
        void containsImage(bool);
        void hasASelectingPhoto(bool);

    public slots:
        void loadImage();
        void pasteImage();
        void save ();
        void reverseH();
        void reverseV();
        void crop();
        void supprPhoto();
        void addBubble();
        void updateActivePhoto();
        void updateSelectingPhoto(bool);
        void zoomIn();
        void zoomOut();
        void resizePage(int value);

    private:

        // ScroolArea
        QScrollArea* scroll;

        // Zoom ratio
        int zoomRatio;

        // Page
        QImage *page;
        QLabel *labPage;

        // Photos
        QVector<Photo*> tabOfPhoto;
        Photo* activePhoto;

        // Bubbles
        QVector<Bubble*> bubbles;
        Bubble *movingBubble;

        // Painter
        QPainter *m_painter;

        // Point where the user clicks
        QPoint clickPoint;

        // context menu
        QMenu *contextMenu;
        QAction *act_paste;
};

#endif // SUBWIN_H
