#include <QDialog>

#include "SubWin.h"

SubWin::SubWin(QWidget* parent) : QWidget(parent)
{
    hSlider = new QSlider(this);
    hSlider->setOrientation(Qt::Horizontal);
    hSlider->setGeometry(0, 0, 100, 50);
    hSlider->hide();

    vSlider = new QSlider(this);
    vSlider->setGeometry(0, 0, 50, 100);
    vSlider->hide();

    sliderForImage = new QSlider(this);
    sliderForImage->setOrientation(Qt::Horizontal);
    sliderForImage->setGeometry(80, 20, 200, 50);
    sliderForImage->hide();

    m_img = new QImage(800, 600, QImage::Format_ARGB32);
    QColor color(Qt::transparent);
    m_img->fill(color);

    reversedHorizontally = false;
    reversedVertically = false;
    croped = false;
    cropDimensions = QRect(0, 0, 800, 600);

    loadedImage = new QImage;

    m_painter = new QPainter;

    m_lab = new QLabel(this);

    m_lab->setPixmap(QPixmap::fromImage(*m_img));

    m_lab->setGeometry(80, 80, m_img->width(), m_img->height());
    setGeometry(0, 0, 800, 600);

    movingBubble = nullptr;
    editingBubble = nullptr;

    connect(sliderForImage, SIGNAL(valueChanged(int)), this, SLOT(resizeWidth(int)));
}

SubWin::~SubWin()
{
    delete loadedImage;
    delete m_painter;
    delete m_img;
    delete m_lab;

    delete hSlider;

    for (int i = 0; i < bubbles.length(); i++)
        delete bubbles[i];

    delete sliderForImage;
}

void SubWin::save ()
{
    QImage imgToSave(*m_img);
    m_painter->begin(&imgToSave);
    for (int i = 0; i < bubbles.length(); i++)
        m_painter->drawPixmap(bubbles[i]->x(), bubbles[i]->y(), QPixmap::fromImage(bubbles[i]->createFinalImage()));
    m_painter->end();
    QString fname = QFileDialog::getSaveFileName(this, nullptr, nullptr, ".png");
    imgToSave.save(fname, "PNG");
}

void SubWin::reverseH ()
{
    QImage *timg = new QImage;
    *timg = m_img->mirrored(true, false);
    delete m_img;
    m_img = timg;
    m_lab->setPixmap(QPixmap::fromImage(*m_img));

    if (reversedHorizontally)
        reversedHorizontally = false;
    else
        reversedHorizontally = true;
}

void SubWin::reverseV ()
{
    QImage *timg = new QImage;
    *timg = m_img->mirrored(false, true);
    delete m_img;
    m_img = timg;
    m_lab->setPixmap(QPixmap::fromImage(*m_img));

    if (reversedVertically)
        reversedVertically = false;
    else
        reversedVertically = true;
}

void SubWin::moveElement (QMouseEvent* event, QPoint relativePos)
{
    QPoint point = mapFromGlobal(event->globalPos());

    if (movingBubble != nullptr)
    {
        disconnect(hSlider, SIGNAL(valueChanged(int)), movingBubble, SLOT(resizeWidth(int)));
        disconnect(vSlider, SIGNAL(valueChanged(int)), movingBubble, SLOT(resizeHeight(int)));
    }

    movingBubble = qobject_cast<Bubble*>(sender());

    //activeBubble->move(point.x() - event->x(), point.y() - event->y());
    movingBubble->move(point.x() - relativePos.x() , point.y() - relativePos.y());

    hSlider->move(movingBubble->x(), movingBubble->y()+movingBubble->height());
    hSlider->setValue((movingBubble->width()-MIN_SIZE_W)/10);
    hSlider->raise();
    hSlider->show();
    connect(hSlider, SIGNAL(valueChanged(int)), movingBubble, SLOT(resizeWidth(int)));

    vSlider->move(movingBubble->x()-vSlider->width(), movingBubble->y()+movingBubble->height()-vSlider->height());
    vSlider->setValue((movingBubble->height()-MIN_SIZE_H)/10);
    vSlider->raise();
    vSlider->show();
    connect(vSlider, SIGNAL(valueChanged(int)), movingBubble, SLOT(resizeHeight(int)));
}

void SubWin::addBubble()
{
    Bubble* newBubble = new Bubble(this);
    bubbles.push_back(newBubble);

    newBubble->show();

    connect(newBubble, SIGNAL(grabbed(QMouseEvent*, QPoint)), this, SLOT(moveElement(QMouseEvent*, QPoint)));
    connect(newBubble, SIGNAL(editing()), this, SLOT(updateEditingBubble()));
}

void SubWin::loadImage()
{
    QString fname = QFileDialog::getOpenFileName(this);

    if (fname != nullptr)
    {
        if (loadedImage->load(fname))
        {
            delete m_img;
            m_img = new QImage(*loadedImage);

            reversedHorizontally = false;
            reversedVertically = false;
            croped = false;

            m_lab->setPixmap(QPixmap::fromImage(*m_img));
            m_lab->setGeometry(80, 80, m_img->width(), m_img->height());
            sliderForImage->setValue((m_img->width() - MIN_IMG_SIZE_W)/35);
        }
    }

    cropDimensions.setX(0);
    cropDimensions.setY(0);
    cropDimensions.setWidth(m_img->width());
    cropDimensions.setHeight(m_img->height());
}

void SubWin::mouseDoubleClickEvent (QMouseEvent*)
{
    if (editingBubble != nullptr)
        editingBubble->setInactive();

    sliderForImage->setValue((m_img->width() - MIN_IMG_SIZE_W)/35);
    sliderForImage->show();
}

void SubWin::updateEditingBubble()
{
    editingBubble = qobject_cast<Bubble*>(sender());
}

void SubWin::resizeWidth(int value)
{
    *m_img = *loadedImage;

    if (reversedHorizontally)
        *m_img = m_img->mirrored(true, false);

    if (reversedVertically)
        *m_img = m_img->mirrored(false, true);

    if (croped)
        *m_img = m_img->copy(cropDimensions);

    *m_img = m_img->scaled(MIN_IMG_SIZE_W+35*value, 2500, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_lab->setGeometry(80, 80, m_img->width(), m_img->height());
    m_lab->setPixmap(QPixmap::fromImage(*m_img));

}

void SubWin::crop (QRect userCrop)
{
    if ( ! croped )
    {
        cropDimensions.setX(userCrop.x()*loadedImage->width()/m_img->width());
        cropDimensions.setY(userCrop.y()*loadedImage->height()/m_img->height());
        cropDimensions.setWidth(userCrop.width()*loadedImage->width()/m_img->width());
        cropDimensions.setHeight(userCrop.height()*loadedImage->height()/m_img->height());
    }
    else
    {
        /*
        cropDimensions.setX(d.x()*loadedImage->width()/cropDimensions.width());
        cropDimensions.setY(d.y()*loadedImage->height()/cropDimensions.height());
        cropDimensions.setWidth(d.width()*loadedImage->width()/cropDimensions.width());
        cropDimensions.setHeight(d.height()*loadedImage->height()/cropDimensions.height());
        */

        cropDimensions.setX(cropDimensions.x()+userCrop.x()*cropDimensions.width()/m_img->width());
        cropDimensions.setY(cropDimensions.y()+userCrop.y()*cropDimensions.height()/m_img->height());
        cropDimensions.setWidth(userCrop.width()*cropDimensions.width()/m_img->width());
        cropDimensions.setHeight(userCrop.height()*cropDimensions.height()/m_img->height());
    }

    *m_img = m_img->copy(userCrop);

    m_lab->setGeometry(80, 80, m_img->width(), m_img->height());
    m_lab->setPixmap(QPixmap::fromImage(*m_img));

    croped = true;

    sliderForImage->setValue((m_img->width() - MIN_IMG_SIZE_W)/35);
}

void SubWin::crop ()
{
    if ( ! croped )
        crop(QRect(100, 100, 350, 350));
}

