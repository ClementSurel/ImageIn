#include <iostream>

#include "Photo.h"



Photo::Photo(QWidget *parent) : QLabel(parent),
                                selection(QRubberBand::Rectangle, this),
                                topLeftGrip(this, Grip::topLeft),
                                topRightGrip(this, Grip::topRight),
                                bottomLeftGrip(this, Grip::bottomLeft),
                                bottomRightGrip(this, Grip::bottomRight)
{
    isActive = true;
    // Image
    loadedImage = new QImage;
    printedImage = new QImage;

    // context menu
    contextMenu = new QMenu;
    act_reverseH = new QAction("Reverse horizontally");
    act_reverseV = new QAction("Reverse vertically");
    act_crop = new QAction("Crop");
    act_suppr = new QAction("Suppress");
    contextMenu->addAction(act_reverseH);
    contextMenu->addAction(act_reverseV);
    contextMenu->addAction(act_crop);
    contextMenu->addAction(act_suppr);
    //act_lower = new QAction("Lower");
    //contextMenu->addAction(act_lower);

    // actions realised on the image
    reversedHorizontally = false;
    reversedVertically = false;
    croped = false;
    // selection
    selecting = false;

    setWindowFlag(Qt::SubWindow);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Connections with subwindow
    connect(this, SIGNAL(isSelecting(bool)), parent, SLOT(updateSelectingPhoto(bool)));
    connect(this, SIGNAL(activated()), parent, SLOT(updateActivePhoto()));
    connect(act_suppr, SIGNAL(triggered()), parent, SLOT(supprPhoto()));
    //connect(act_lower, SIGNAL(triggered()), this, SLOT(lower()));
    connect(act_reverseH, SIGNAL(triggered()), this, SLOT(reverseH()));
    connect(act_reverseV, SIGNAL(triggered()), this, SLOT(reverseV()));
    connect(act_crop, SIGNAL(triggered()), this, SLOT(crop()));
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

bool Photo::loadImage()
{
    QString fname = QFileDialog::getOpenFileName(this);
    if (fname != nullptr)
    {
        if (loadedImage->load(fname))
        {
            *printedImage = *loadedImage;
            setPixmap(QPixmap::fromImage(*printedImage));
            setGeometry(0, 0, printedImage->width(), printedImage->height());
            reversedHorizontally = false;
            reversedVertically = false;
            croped = false;

            show();

            // grip positions
            topLeftGrip.move(0, 0);
            topRightGrip.move(width()-topRightGrip.width(), 0);
            bottomLeftGrip.move(0, height()-bottomRightGrip.height());
            bottomRightGrip.move(width()-topRightGrip.width(), height()-bottomRightGrip.height());

            return true;
        }
    }

    return false;
}

void Photo::reverseH ()
{
    QImage newImage = printedImage->mirrored(true, false);
    if (!newImage.isNull())
    {
        *printedImage = newImage;
        setPixmap(QPixmap::fromImage(*printedImage));

        reversedHorizontally = !reversedHorizontally;
    }
}

void Photo::reverseV ()
{
    *printedImage = printedImage->mirrored(false, true);
    setPixmap(QPixmap::fromImage(*printedImage));

    reversedVertically = !reversedVertically;
}

QImage Photo::finalImage ()
{
     return *printedImage;
}

// Mouse Events
void Photo::mouseMoveEvent(QMouseEvent *event)
{
    if ( !(event->buttons() & Qt::LeftButton) )
        return;

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
    if (!isActive)
    {
        emit activated();
        isActive = true;
    }

    selecting = true;
    emit isSelecting(true);

    clickPoint.setX(event->x());
    clickPoint.setY(event->y());
    selection.setGeometry(clickPoint.x(), clickPoint.y(), 0, 0);
}

void Photo::mousePressEvent (QMouseEvent *event)
{
    if (!isActive)
    {
        emit activated();
        isActive = true;
    }

    mouseButton = event->button();
    relativePos = mapFromGlobal(event->globalPos());

    if (event->button() == Qt::LeftButton)
    {
        selecting = false;
        emit isSelecting(false);
        selection.hide();
    }
}

void Photo::contextMenuEvent(QContextMenuEvent *event)
{
    if (!isActive)
    {
        emit activated();
        isActive = true;
    }

    act_crop->setEnabled (selecting ? true : false);

    contextMenu->move(event->globalPos());
    contextMenu->show();
}

// Resizing methods
void Photo::resizeImage(int newWidth, int newHeight)
{
    QImage newImage;
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
    newImage = printedImage->scaled(newWidth, newHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    if (!newImage.isNull())
        *printedImage = newImage;
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

        *printedImage = printedImage->copy(selection.x(), selection.y(), selection.width(), selection.height());

        setGeometry(mapToParent(selection.pos()).x(), mapToParent(selection.pos()).y(), printedImage->width(), printedImage->height());
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

        *printedImage = printedImage->copy(selection.x(), selection.y(), selection.width(), selection.height());

        selection.hide();

        setGeometry(mapToParent(selection.pos()).x(), mapToParent(selection.pos()).y(), printedImage->width(), printedImage->height());
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
