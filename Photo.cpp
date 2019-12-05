#include <iostream>
#include "Photo.h"

Photo::Photo(QWidget *parent) : QLabel(parent),
                                selection(QRubberBand::Rectangle, this),
                                topLeftGrip(this, Grip::topLeft),
                                topRightGrip(this, Grip::topRight),
                                bottomLeftGrip(this, Grip::bottomLeft),
                                bottomRightGrip(this, Grip::bottomRight)
{
    loadedImage = new QImage;
    printedImage = new QImage;

    // QActions
    contextMenu = new QMenu;
    act_crop = new QAction("Crop");
    //act_lower = new QAction("Lower");
    contextMenu->addAction(act_crop);
    //contextMenu->addAction(act_lower);

    reversedHorizontally = false;
    reversedVertically = false;
    croped = false;

    coord.setX(0);
    coord.setY(0);

    selecting = false;

    setWindowFlag(Qt::SubWindow);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    coord.setX(x());
    coord.setY(y());

    locked = false;

    //connect(act_lower, SIGNAL(triggered()), this, SLOT(lower()));
    connect(act_crop, SIGNAL(triggered()), parent, SLOT(crop()));
    connect(&topLeftGrip, SIGNAL(grabbed(QMouseEvent*, Grip::Corner)), this, SLOT(resizeEverything(QMouseEvent*, Grip::Corner)));
    connect(&topRightGrip, SIGNAL(grabbed(QMouseEvent*, Grip::Corner)), this, SLOT(resizeEverything(QMouseEvent*, Grip::Corner)));
    connect(&bottomLeftGrip, SIGNAL(grabbed(QMouseEvent*, Grip::Corner)), this, SLOT(resizeEverything(QMouseEvent*, Grip::Corner)));
    connect(&bottomRightGrip, SIGNAL(grabbed(QMouseEvent*, Grip::Corner)), this, SLOT(resizeEverything(QMouseEvent*, Grip::Corner)));
}

Photo::~Photo()
{
    //delete act_lower;
    delete act_crop;
    delete contextMenu;

    delete loadedImage;
    delete printedImage;
}

void Photo::loadImage()
{
    QString fname = QFileDialog::getOpenFileName(this);

    if (fname != nullptr)
    {
        if (loadedImage->load(fname))
        {
            *printedImage = *loadedImage;
            setPixmap(QPixmap::fromImage(*printedImage));
            setGeometry(0, 0, printedImage->width(), printedImage->height());
            show();
            reversedHorizontally = false;
            reversedVertically = false;
            croped = false;

            // grip positions
            topLeftGrip.move(0, 0);
            topRightGrip.move(width()-topRightGrip.width(), 0);
            bottomLeftGrip.move(0, height()-bottomRightGrip.height());
            bottomRightGrip.move(width()-topRightGrip.width(), height()-bottomRightGrip.height());
        }
    }

}

void Photo::reverseH ()
{
    *printedImage = printedImage->mirrored(true, false);
    setPixmap(QPixmap::fromImage(*printedImage));

    if (reversedHorizontally)
        reversedHorizontally = false;
    else
        reversedHorizontally = true;
}

void Photo::reverseV ()
{
    *printedImage = printedImage->mirrored(false, true);
    setPixmap(QPixmap::fromImage(*printedImage));

    if (reversedVertically)
        reversedVertically = false;
    else
        reversedVertically = true;
}

QImage Photo::finalImage ()
{
     return *printedImage;
}

void Photo::mouseMoveEvent(QMouseEvent *event)
{
    if (selecting)
    {
        QRect selectionRect;
        if ( event->x() > clickPoint.x() )
        {
            selectionRect.setX(clickPoint.x());
            selectionRect.setWidth(event->x()-clickPoint.x());
            if (selectionRect.x() + selectionRect.width() > this->width())
                selectionRect.setWidth(this->width()-selectionRect.x());
        }
        else
        {
            selectionRect.setX(event->x());
            selectionRect.setWidth(clickPoint.x()-event->x());
            if (selectionRect.x() < 0)
                selectionRect.setX(0);
        }

        if ( event->y() > clickPoint.y() )
        {
            selectionRect.setY(clickPoint.y());
            selectionRect.setHeight(event->y()-clickPoint.y());
            if (selectionRect.y() + selectionRect.height() > this->height())
                selectionRect.setHeight(this->height()-selectionRect.y());
        }
        else
        {
            selectionRect.setY(event->y());
            selectionRect.setHeight(clickPoint.y()-event->y());
            if (selectionRect.y() < 0)
                selectionRect.setY(0);
        }

        selection.setGeometry(selectionRect);
        selection.show();
    }

    else if (mouseButton == Qt::LeftButton)
    {
        QPoint point = mapFromGlobal(event->globalPos());
        point = mapToParent(point);

        move(point.x() - relativePos.x(), point.y() - relativePos.y());
    }
}

void Photo::mouseDoubleClickEvent(QMouseEvent *event)
{
    selecting = true;
    clickPoint.setX(event->x());
    clickPoint.setY(event->y());
    selection.setGeometry(clickPoint.x(), clickPoint.y(), 0, 0);
}

void Photo::mousePressEvent (QMouseEvent *event)
{
    mouseButton = event->button();
    relativePos = mapFromGlobal(event->globalPos());

    if (event->button() == Qt::LeftButton)
    {
        selecting = false;
        selection.hide();
    }
}

void Photo::contextMenuEvent(QContextMenuEvent *event)
{
    contextMenu->clear();

    if (selecting)
           contextMenu->addAction(act_crop);

    contextMenu->move(event->globalPos());
    contextMenu->show();
}

void Photo::resizeImage(int newWidth, int newHeight)
{
    // Restart from the original image
    *printedImage = *loadedImage;

    // Apply the transformation
    if (reversedHorizontally)
        *printedImage = printedImage->mirrored(true, false);
    if (reversedVertically)
        *printedImage = printedImage->mirrored(false, true);
    if (croped)
        *printedImage = printedImage->copy(cropRect);

    // Rescale the image
    *printedImage = printedImage->scaled(newWidth, newHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void Photo::crop ()
{
    if ( ! croped && selecting)
    {
        cropRect.setX(selection.x()*loadedImage->width()/printedImage->width());
        cropRect.setY(selection.y()*loadedImage->height()/printedImage->height());
        cropRect.setWidth(selection.width()*loadedImage->width()/printedImage->width());
        cropRect.setHeight(selection.height()*loadedImage->height()/printedImage->height());

        croped = true;

        selecting = false;
        selection.hide();

        coord.setX(x());
        coord.setY(y());

        *printedImage = printedImage->copy(selection.x(), selection.y(), selection.width(), selection.height());

        setGeometry(coord.x(), coord.y(), printedImage->width(), printedImage->height());
        setPixmap(QPixmap::fromImage(*printedImage));
    }
    else if (selecting)
    {
        int width = cropRect.width();
        int height = cropRect.height();
        cropRect.setX(cropRect.x()+selection.x()*cropRect.width()/printedImage->width());
        cropRect.setY(cropRect.y()+selection.y()*cropRect.height()/printedImage->height());
        cropRect.setWidth(selection.width()*width/printedImage->width());
        cropRect.setHeight(selection.height()*height/printedImage->height());

        selecting = false;

        coord.setX(x());
        coord.setY(y());

        *printedImage = printedImage->copy(selection.x(), selection.y(), selection.width(), selection.height());

        selection.hide();

        setGeometry(coord.x(), coord.y(), printedImage->width(), printedImage->height());
        setPixmap(QPixmap::fromImage(*printedImage));
    }

}

void Photo::resizeEvent(QResizeEvent*)
{
    topLeftGrip.move(0, 0);
    topRightGrip.move(width()-topRightGrip.width(), 0);
    bottomLeftGrip.move(0, height()-bottomRightGrip.height());
    bottomRightGrip.move(width()-topRightGrip.width(), height()-bottomRightGrip.height());
}

void Photo::resizeEverything (QMouseEvent *e, Grip::Corner corner)
{
    // Determinate the cursor position
    QPoint point = mapFromGlobal(e->globalPos());
    point = mapToParent(point);

    // Resize the image if the final image dimensions is superior to the minimun size
    int newX, newY, newWidth, newHeight;
    switch (corner)
    {
        case Grip::topLeft:
            newWidth = x()-point.x()+width();
            newHeight = y()-point.y()+height();
            if (newWidth < 60 || newHeight < 60)
                return;
            resizeImage(newWidth, newHeight);
            newX = x()+width()-printedImage->width();
            newY = y()+height()-printedImage->height();
            break;
        case Grip::topRight:
            newWidth = point.x()-x();
            newHeight = y()-point.y()+height();
            if (newWidth < 60 || newHeight < 60)
                return;
            resizeImage(newWidth, newHeight);
            newX = x();
            newY = y()+height()-printedImage->height();
            break;
        case Grip::bottomLeft:
            newWidth = x()-point.x()+width();
            newHeight = point.y()-y();
            if (newWidth < 60 || newHeight < 60)
                return;
            resizeImage(newWidth, newHeight);
            newX = x()+width()-printedImage->width();
            newY = y();
            break;
        case Grip::bottomRight:
            newWidth = point.x()-x();
            newHeight = point.y()-y();
            if (newWidth < 60 || newHeight < 60)
                return;
            resizeImage(newWidth, newHeight);
            newX = x();
            newY = y();
            break;
    }
    setGeometry(newX, newY, printedImage->width(), printedImage->height());

    // Update printedImage
    setPixmap(QPixmap::fromImage(*printedImage));
}
