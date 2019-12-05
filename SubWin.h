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
    void moveElement(QMouseEvent*, QPoint);
    void movePhoto (QMouseEvent* event, QPoint relativePos);
    void addBubble();
    void updateEditingBubble();
    void resizeWidth(int value);
    void crop();

private:
    // Page
    QImage *page;
    QLabel *labPage;

    // Images
    //QLabel *m_lab;

    Photo* activePhoto;
    QVector<Photo*> tabOfPhoto;

    QPainter *m_painter;

    QPoint relativePos;

    QVector<Bubble*> bubbles;
    Bubble *movingBubble;
    Bubble *editingBubble;

    QSlider *sliderForImage;
};

#endif // SUBWIN_H
