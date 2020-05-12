#include "bubble.h"

Bubble::Bubble(int x, int y, int ratio, QWidget* parent) :  QLabel (parent),
                                                            topLeftGrip(this, Grip::topLeft),
                                                            topRightGrip(this, Grip::topRight),
                                                            downLeftGrip(this, Grip::bottomLeft),
                                                            downRightGrip(this, Grip::bottomRight)
{
    zoom = ratio;

    // Create the painter object
    painter = new QPainter;

    // Draw the bubble image
    img = new QImage(NATIVE_SIZE_W, NATIVE_SIZE_H, QImage::Format_ARGB32);

    QColor color(Qt::transparent);
    img->fill(color);

    painter->begin(img);
    painter->setRenderHints(QPainter::Antialiasing, true);

    QPainterPath path(QPointF(0, 0));
    path.addEllipse(QRectF(MARGIN_BUBBLE*zoom/100, MARGIN_BUBBLE*zoom/100,
                           img->width()-(2*MARGIN_BUBBLE*zoom/100), img->height()-(2*MARGIN_BUBBLE*zoom/100)));

    painter->drawPath(path);
    painter->fillPath(path, QBrush(Qt::white));
    painter->end();

    // Set up the editing text
    editingText = new QTextEdit(this);
    editingText->setGeometry(20, 20, img->width()-40, img->height()-40);
    editingText->setAlignment(Qt::AlignCenter);
    editingText->setFont(QFont("Comic Sans MS", DEFAULT_FONT_POINTSIZE*ratio/100));
    editingText->setVisible(false);

    // Set up the context Menu
    contextMenu = new QMenu(this);
    act_raise = new QAction("raise", this);
    contextMenu->addAction(act_raise);

    connect(act_raise, SIGNAL(triggered()), this, SLOT(raise()));

    // Set up the widget properties
    setWindowFlag(Qt::SubWindow); // To allow resizing with grips
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(MIN_SIZE_W, MIN_SIZE_H);

    realX = x*100/zoom;
    realY = y*100/zoom;
    realWidth = img->width()*100/zoom;
    realHeight = img->height()*100/zoom;

    // Display the bubble image
    setPixmap(QPixmap::fromImage(*img));
    setGeometry(x, y, img->width(), img->height());
    show();

    // Position the grips
    topLeftGrip.move(0, 0);
    topRightGrip.move(width()-topRightGrip.width(), 0);
    downLeftGrip.move(0, height()-downLeftGrip.height());
    downRightGrip.move(width()-downRightGrip.width(), height()-downRightGrip.height());

    connect(&topLeftGrip, SIGNAL(grabbed(QMouseEvent*, Grip::Corner)), this, SLOT(resizeWithGrip(QMouseEvent*, Grip::Corner)));
    connect(&topRightGrip, SIGNAL(grabbed(QMouseEvent*, Grip::Corner)), this, SLOT(resizeWithGrip(QMouseEvent*, Grip::Corner)));
    connect(&downLeftGrip, SIGNAL(grabbed(QMouseEvent*, Grip::Corner)), this, SLOT(resizeWithGrip(QMouseEvent*, Grip::Corner)));
    connect(&downRightGrip, SIGNAL(grabbed(QMouseEvent*, Grip::Corner)), this, SLOT(resizeWithGrip(QMouseEvent*, Grip::Corner)));
}

Bubble::~Bubble()
{
    delete painter;
    delete img;

    delete editingText;

    delete contextMenu;
    delete act_raise;
}

QImage Bubble::createFinalImage(int ratio)
{    
    /*
    // Create final Image object
    QImage finalImage (img->width()*100/ratio, img->height()*100/ratio, QImage::Format_ARGB32);

    // Fill it with transparent color
    QColor color(Qt::transparent);
    finalImage.fill(color);

    // Draw the white bubble
    painter->begin(&finalImage);
    painter->setRenderHints(QPainter::Antialiasing, true);

    QPainterPath path(QPointF(0, 0));
    path.addEllipse(QRectF(10*100/ratio, 10*100/ratio,
                           finalImage.width()-20*100/ratio, finalImage.height()-20*100/ratio));

    painter->setPen(Qt::black);
    painter->drawPath(path);
    painter->fillPath(path, QBrush(Qt::white));

    // Draw the text
    QFont newFont = editingText->font();
    newFont.setPointSize(newFont.pointSize()*100/ratio);

    painter->setFont(newFont);

    QTextOption textOp;
    textOp.setWrapMode(QTextOption::WordWrap);
    textOp.setAlignment(Qt::AlignCenter);

    painter->drawText(QRectF(editingText->x()*100/ratio, editingText->y()*100/ratio,
                             editingText->width()*100/ratio, editingText->height()*100/ratio),
                             editingText->toPlainText(), textOp);

    painter->end();

    return finalImage;
    */

    return resizeBubble(realWidth, realHeight);
}

// Mouse events
void Bubble::mousePressEvent (QMouseEvent *event)
{
    clickPosistion = mapFromGlobal(event->globalPos());
}

/*
void Bubble::mouseReleaseEvent (QMouseEvent *event)
{
    event->setAccepted(true);

    editingText->setVisible(false);

    // Repaint the image
    // Draw the bubble
    QPainterPath path(QPointF(0, 0));
    path.addEllipse(QRectF(10, 10, img->width()-20, img->height()-20));

    painter->begin(img);
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->setPen(Qt::black);
    painter->drawPath(path);
    painter->fillPath(path, QBrush(Qt::white));

    // Draw the text
    painter->setFont(editingText->font());

    QTextOption textOp;
    textOp.setWrapMode(QTextOption::WordWrap);
    textOp.setAlignment(Qt::AlignCenter);

    painter->drawText(QRectF(editingText->x(), editingText->y(),
                             editingText->width(), editingText->height()),
                             editingText->toPlainText(), textOp);

    painter->end();

    setPixmap(QPixmap::fromImage(*img));
}
*/

void Bubble::mouseMoveEvent (QMouseEvent *event)
{
    if ( (event->buttons() & Qt::RightButton) == Qt::RightButton)
        return;

    QPoint point = mapFromGlobal(event->globalPos());
    point = mapToParent(point);

    move(point.x() - clickPosistion.x() , point.y() - clickPosistion.y());

    realX = x()*100/zoom;
    realY = y()*100/zoom;
}

void Bubble::mouseDoubleClickEvent (QMouseEvent *event)
{
    event->setAccepted(true);

    if (editingText->isVisible() == false)
    {
        editingText->setVisible(true);
        grabMouse();
        editingText->grabKeyboard();
    }
    else
    {
        editingText->setVisible(false);

        // Repaint the image
        // Draw the bubble
        QPainterPath path(QPointF(0, 0));
        path.addEllipse(QRectF(MARGIN_BUBBLE*zoom/100, MARGIN_BUBBLE*zoom/100,
                               img->width()-(2*MARGIN_BUBBLE*zoom/100), img->height()-(2*MARGIN_BUBBLE*zoom/100)));
        painter->begin(img);
        painter->setRenderHints(QPainter::Antialiasing, true);
        painter->setPen(Qt::black);
        painter->drawPath(path);
        painter->fillPath(path, QBrush(Qt::white));

        // Draw the text
        painter->setFont(editingText->font());

        QTextOption textOp;
        textOp.setWrapMode(QTextOption::WordWrap);
        textOp.setAlignment(Qt::AlignCenter);

        painter->drawText(QRectF(editingText->x(), editingText->y(),
                                 editingText->width(), editingText->height()),
                                 editingText->toPlainText(), textOp);

        painter->end();

        setPixmap(QPixmap::fromImage(*img));

        editingText->releaseKeyboard();
        releaseMouse();
    }
}

void Bubble::contextMenuEvent(QContextMenuEvent *event)
{
    contextMenu->move(event->globalPos());
    contextMenu->show();
}

// Resize
void Bubble::resizeWithZoom(int ratio)
{
    zoom = ratio;

    // Change the font pointsize
    QFont newFont = editingText->font();
    newFont.setPointSize(DEFAULT_FONT_POINTSIZE*zoom/100);
    editingText->setFont(newFont);

    // Resize everything
    setGeometry(realX*zoom/100, realY*zoom/100, realWidth*zoom/100, realHeight*zoom/100);
}

void Bubble::resizeWithGrip (QMouseEvent *e, Grip::Corner c)
{
    int newX, newY, newWidth, newHeight;

    QPoint mousePoint = mapFromGlobal(e->globalPos());
    mousePoint = mapToParent(mousePoint);

    switch (c)
    {
        case Grip::topLeft:
            newWidth = x() - mousePoint.x() + width();
            newHeight = y() - mousePoint.y() + height();
            newX = mousePoint.x();
            newY = mousePoint.y();
            break;
        case Grip::topRight:
            newWidth = mousePoint.x() - x();
            newHeight = y() - mousePoint.y() + height();
            newX = x();
            newY = mousePoint.y();
            break;
        case Grip::bottomLeft:
            newWidth = x() - mousePoint.x() + width();
            newHeight = mousePoint.y() - y();
            newX = mousePoint.x();
            newY = y();
            break;
        case Grip::bottomRight:
            newWidth = mousePoint.x() - x();
            newHeight = mousePoint.y() - y();
            newX = x();
            newY = y();
            break;
    }

    // Update real Position and Dimensions
    realX = newX*100/zoom;
    realY = newY*100/zoom;
    realWidth = newWidth*100/zoom;
    realHeight = newHeight*100/zoom;

    setGeometry(newX, newY, newWidth, newHeight);
}

QImage Bubble::resizeBubble (int newWidth, int newHeight)
{
    // Repaint the image
    QImage resizedImage(newWidth, newHeight, QImage::Format_ARGB32);
    QColor color(Qt::transparent);
    resizedImage.fill(color);

    painter->begin(&resizedImage);
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->setPen(Qt::black);

    QPainterPath path(QPointF(0, 0));
    path.addEllipse(QRectF(MARGIN_BUBBLE*zoom/100, MARGIN_BUBBLE*zoom/100,
                           resizedImage.width()-(2*MARGIN_BUBBLE*zoom/100), resizedImage.height()-(2*MARGIN_BUBBLE*zoom/100)));
    painter->drawPath(path);
    painter->fillPath(path, QBrush(Qt::white));

    painter->setFont(editingText->font());

    QTextOption textOp;
    textOp.setWrapMode(QTextOption::WordWrap);
    textOp.setAlignment(Qt::AlignCenter);

    // Resize and reposition the text
    editingText->setGeometry(20, 20, resizedImage.width()-40, resizedImage.height()-40);

    // Draw the text
    painter->drawText(QRectF(editingText->x(), editingText->y(),
                             editingText->width(), editingText->height()),
                             editingText->toPlainText(), textOp);

    painter->end();

    return resizedImage;
}

void Bubble::resizeEvent (QResizeEvent*)
{
    delete img;
    img = new QImage(resizeBubble(width(), height()));

    setPixmap(QPixmap::fromImage(*img));

    // Reposition grips
    topLeftGrip.move(0, 0);
    topRightGrip.move(width()-topRightGrip.width(), 0);
    downLeftGrip.move(0, height()-downLeftGrip.height());
    downRightGrip.move(width()-downRightGrip.width(), height()-downRightGrip.height());
}

