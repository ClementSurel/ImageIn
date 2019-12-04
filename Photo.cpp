#include "Photo.h"

Photo::Photo(QWidget *parent) : QLabel(parent), selection(QRubberBand::Rectangle, this), topLeftGrip(this)
{
    loadedImage = new QImage;
    printedImage = new QImage;

    // QActions
    contextMenu = new QMenu;
    act_crop = new QAction("Crop");
    act_lower = new QAction("Lower");
    contextMenu->addAction(act_crop);
    contextMenu->addAction(act_lower);

    reversedHorizontally = false;
    reversedVertically = false;
    croped = false;

    setGeometry(0, 0, 10, 10);

    setStyleSheet("QLabel { background-color : red; color : blue; }");
    /*
    QPainter painter;
    painter.begin(this);
    painter.fillRect(0, 0, this->width(), this->height(), QColor(Qt::red));
    painter.end();
    */

    coord.setX(0);
    coord.setY(0);

    selecting = false;

    setWindowFlag(Qt::SubWindow);

    connect(act_lower, SIGNAL(triggered()), this, SLOT(lower()));
    connect(act_crop, SIGNAL(triggered()), parent, SLOT(crop()));
}

Photo::~Photo()
{
    delete act_lower;
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
            topLeftGrip.move(this->width()/2, this->height()/2);
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
        emit grabbed(event, relativePos);
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
    contextMenu->move(event->globalPos());
    contextMenu->show();
}

void Photo::resize(int value)
{
    coord.setX(x());
    coord.setY(y());

    if (coord.x() < 0)
        coord.setX(0);
    if (coord.y() < 0)
        coord.setY(0);

    *printedImage = *loadedImage;

    if (reversedHorizontally)
        *printedImage = printedImage->mirrored(true, false);

    if (reversedVertically)
        *printedImage = printedImage->mirrored(false, true);

    if (croped)
        *printedImage = printedImage->copy(cropRect);

    *printedImage = printedImage->scaled(MIN_IMG_SIZE_W+35*value, 3508, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    setGeometry(coord.x(), coord.y(), printedImage->width(), printedImage->height());
    setPixmap(QPixmap::fromImage(*printedImage));
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
    setPixmap(QPixmap::fromImage(*printedImage));
}
