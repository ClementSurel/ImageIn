#include "SubWin.h"
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

    realX = 0;
    realY = 0;
    realWidth = 0;
    realHeight = 0;

    // context menu
    contextMenu = new QMenu;
    act_reverseH = new QAction("Reverse horizontally");
    act_reverseV = new QAction("Reverse vertically");
    act_crop = new QAction("Crop");
    act_raise = new QAction("Raise");
    act_suppr = new QAction("Suppress");

    contextMenu->addAction(act_reverseH);
    contextMenu->addAction(act_reverseV);
    contextMenu->addAction(act_crop);
    contextMenu->addAction(act_raise);
    contextMenu->addAction(act_suppr);
    //act_lower = new QAction("Lower");
    //contextMenu->addAction(act_lower);

    // image transformations
    reversedHorizontally = false;
    reversedVertically = false;
    croped = false;

    // selection
    selecting = false;

    // Set up widget properties
    setWindowFlag(Qt::SubWindow); // To allow resizing with grips
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Connections with subwindow
    connect(this, SIGNAL(isSelecting(bool)), parent, SLOT(updateSelectingPhoto(bool)));
    connect(this, SIGNAL(activated()), parent, SLOT(updateActivePhoto()));

    // Connections between actions of the context menu and the corresponding functions
    connect(act_reverseH, SIGNAL(triggered()), this, SLOT(reverseH()));
    connect(act_reverseV, SIGNAL(triggered()), this, SLOT(reverseV()));
    connect(act_crop, SIGNAL(triggered()), this, SLOT(crop()));
    connect(act_raise, SIGNAL(triggered()), this, SLOT(raise()));
    //connect(act_lower, SIGNAL(triggered()), this, SLOT(lower()));
    connect(act_suppr, SIGNAL(triggered()), parent, SLOT(supprPhoto()));

    // Connections between grips and the image
    connect(&topLeftGrip, SIGNAL(grabbed(QMouseEvent*, Grip::Corner)), this, SLOT(resizeWithGrip(QMouseEvent*, Grip::Corner)));
    connect(&topRightGrip, SIGNAL(grabbed(QMouseEvent*, Grip::Corner)), this, SLOT(resizeWithGrip(QMouseEvent*, Grip::Corner)));
    connect(&bottomLeftGrip, SIGNAL(grabbed(QMouseEvent*, Grip::Corner)), this, SLOT(resizeWithGrip(QMouseEvent*, Grip::Corner)));
    connect(&bottomRightGrip, SIGNAL(grabbed(QMouseEvent*, Grip::Corner)), this, SLOT(resizeWithGrip(QMouseEvent*, Grip::Corner)));
}

Photo::~Photo()
{
    delete act_reverseH;
    delete act_reverseV;
    //delete act_lower;
    delete act_crop;
    delete act_raise;
    delete act_suppr;
    delete contextMenu;

    delete loadedImage;
    delete printedImage;
}

bool Photo::loadImage(int x, int y, int zoom)
{
    QString fname = QFileDialog::getOpenFileName(this);
    if (fname != nullptr)
    {
        if (loadedImage->load(fname))
        {
            *printedImage = *loadedImage;
            reversedHorizontally = false;
            reversedVertically = false;
            croped = false;

            realWidth = loadedImage->width();
            realHeight = loadedImage->height();
            resizeWithZoom(zoom);

            move(x, y);
            realX = this->x()*100/zoom;
            realY = this->y()*100/zoom;

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

bool Photo::pasteImage(int x, int y, int zoom)
{
    QClipboard *clipboard = qApp->clipboard();

    if (clipboard->mimeData()->hasImage())
    {
        *loadedImage = clipboard->image();
        *printedImage = *loadedImage;
        reversedHorizontally = false;
        reversedVertically = false;
        croped = false;

        realWidth = loadedImage->width();
        realHeight = loadedImage->height();
        resizeWithZoom(zoom);

        move(x, y);
        realX = this->x()*100/zoom;
        realY = this->y()*100/zoom;

        show();

        // grip positions
        topLeftGrip.move(0, 0);
        topRightGrip.move(width()-topRightGrip.width(), 0);
        bottomLeftGrip.move(0, height()-bottomRightGrip.height());
        bottomRightGrip.move(width()-topRightGrip.width(), height()-bottomRightGrip.height());

        return true;
    }

    return false;
}


QImage Photo::finalImage (int zoomRatio)
{
    return resizeImage(realWidth, realHeight);
}


// Transformations

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

void Photo::crop ()
{
    if (selecting)
    {
        if ( ! croped)
        {
            int cropX = selection.x()*loadedImage->width()/printedImage->width();
            int cropY = selection.y()*loadedImage->height()/printedImage->height();
            int cropW = selection.width()*loadedImage->width()/printedImage->width();
            int cropH = selection.height()*loadedImage->height()/printedImage->height();

            if ( reversedHorizontally )
                cropX = loadedImage->width()-cropX-cropW;

            if ( reversedVertically )
                cropY = loadedImage->height()-cropY-cropH;

            cropRect.setX(cropX);
            cropRect.setY(cropY);
            cropRect.setWidth(cropW);
            cropRect.setHeight(cropH);

            croped = true;
        }
        else
        {
            int cropX = selection.x()*cropRect.width()/printedImage->width();
            int cropY = selection.y()*cropRect.height()/printedImage->height();
            int cropW = selection.width()*cropRect.width()/printedImage->width();
            int cropH = selection.height()*cropRect.height()/printedImage->height();

            if ( reversedHorizontally )
                cropX = cropRect.width()-cropX-cropW;
            if ( reversedVertically )
                cropY = cropRect.height()-cropY-cropH;

            cropRect.setX(cropRect.x() + cropX);
            cropRect.setY(cropRect.y() + cropY);
            cropRect.setWidth(cropW);
            cropRect.setHeight(cropH);
        }

        selecting = false;
        emit isSelecting(false);
        selection.hide();

        *printedImage = printedImage->copy(selection.x(), selection.y(), selection.width(), selection.height());

        setGeometry(mapToParent(selection.pos()).x(), mapToParent(selection.pos()).y(),
                    printedImage->width(), printedImage->height());

        setPixmap(QPixmap::fromImage(*printedImage));

        realX = x()*100/zoom;
        realY = y()*100/zoom;
        realWidth = width()*100/zoom;
        realHeight = height()*100/zoom;
    }
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

        realX = x()*100/zoom;
        realY = y()*100/zoom;
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

QImage Photo::resizeImage(int newWidth, int newHeight)
{
    QImage resizedImage;

    // Restart from the original image
    resizedImage = *loadedImage;

    // Apply the transformation
    if (croped)
        resizedImage = resizedImage.copy(cropRect);
    if (reversedHorizontally)
        resizedImage = resizedImage.mirrored(true, false);
    if (reversedVertically)
        resizedImage = resizedImage.mirrored(false, true);

    // Rescale the image
    resizedImage = resizedImage.scaled(newWidth, newHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    return resizedImage;
}

void Photo::resizeWithGrip (QMouseEvent *e, Grip::Corner corner)
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
            *printedImage = resizeImage(newWidth, newHeight);
            newX = x()+width()-printedImage->width();
            newY = y()+height()-printedImage->height();
            break;
        case Grip::topRight:
            newWidth = point.x()-x();
            newHeight = y()-point.y()+height();
            if (newWidth < 60 || newHeight < 60)
                return;
            *printedImage = resizeImage(newWidth, newHeight);
            newX = x();
            newY = y()+height()-printedImage->height();
            break;
        case Grip::bottomLeft:
            newWidth = x()-point.x()+width();
            newHeight = point.y()-y();
            if (newWidth < 60 || newHeight < 60)
                return;
            *printedImage = resizeImage(newWidth, newHeight);
            newX = x()+width()-printedImage->width();
            newY = y();
            break;
        case Grip::bottomRight:
            newWidth = point.x()-x();
            newHeight = point.y()-y();
            if (newWidth < 60 || newHeight < 60)
                return;
            *printedImage = resizeImage(newWidth, newHeight);
            newX = x();
            newY = y();
            break;
    }

    setGeometry(newX, newY, printedImage->width(), printedImage->height());

    realX = x()*100/zoom;
    realY = y()*100/zoom;
    realWidth = width()*100/zoom;
    realHeight = height()*100/zoom;

    // Update printedImage
    setPixmap(QPixmap::fromImage(*printedImage));
}

void Photo::resizeEvent(QResizeEvent*)
{
    // Reposition the grips
    topLeftGrip.move(0, 0);
    topRightGrip.move(width()-topRightGrip.width(), 0);
    bottomLeftGrip.move(0, height()-bottomRightGrip.height());
    bottomRightGrip.move(width()-topRightGrip.width(), height()-bottomRightGrip.height());
}

void Photo::resizeWithZoom (int zoom)
{
    this->zoom = zoom;

    delete printedImage;
    printedImage = new QImage(resizeImage(realWidth*zoom/100, realHeight*zoom/100));

    setGeometry(realX*zoom/100, realY*zoom/100, printedImage->width(), printedImage->height());

    // Update printedImage
    setPixmap(QPixmap::fromImage(*printedImage));
}

