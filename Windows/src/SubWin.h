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

signals:
    void containsImage(bool);
    void hasASelectingPhoto(bool);

public slots:
    void loadImage();
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
        QPoint relativePos;
};

#endif // SUBWIN_H
