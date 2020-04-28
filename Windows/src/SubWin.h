#ifndef SUBWIN_H
#define SUBWIN_H

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
    SubWin(QWidget* parent = nullptr);
    ~SubWin();
    void mouseDoubleClickEvent(QMouseEvent*);

public slots:
    void loadImage();
    void save ();
    void reverseH();
    void reverseV();
    void addBubble();
    void updateEditingBubble();
    void crop();

private:
    // Page
    QImage *page;
    QLabel *labPage;

    // Photos
    QVector<Photo*> tabOfPhoto;
    Photo* activePhoto;

    // Bubbles
    QVector<Bubble*> bubbles;
    Bubble *movingBubble;
    Bubble *editingBubble;

    // Painter
    QPainter *m_painter;

    // Point where the user clicks
    QPoint relativePos;
};

#endif // SUBWIN_H
