#include <QDialog>

#include "SubWin.h"

SubWin::SubWin(QWidget* parent) : QWidget(parent)
{
    // Page
    labPage = new QLabel(this);
    setGeometry(0, 0, PAGE_W, PAGE_H);

    page = new QImage(PAGE_W, PAGE_H, QImage::Format_ARGB32);
    QColor color(Qt::lightGray);
    page->fill(color);
    labPage->setPixmap(QPixmap::fromImage(*page));

    // Slider
    sliderForImage = new QSlider(this);
    sliderForImage->setOrientation(Qt::Horizontal);
    sliderForImage->setGeometry(80, 20, 200, 50);
    sliderForImage->hide();

    m_painter = new QPainter;
    m_painter->begin(page);
    m_painter->setPen(QColor(Qt::black));
    QRect line(100, 0, 5, page->height());
    m_painter->fillRect(line, Qt::SolidPattern);
    m_painter->end();
    labPage->setPixmap(QPixmap::fromImage(*page));

    //m_lab = new QLabel(this);

    movingBubble = nullptr;
    editingBubble = nullptr;

    activePhoto = nullptr;

    connect(sliderForImage, SIGNAL(valueChanged(int)), this, SLOT(resizeWidth(int)));
}

SubWin::~SubWin()
{
    delete m_painter;

    //delete m_lab;

    for (int i = 0; i < bubbles.length(); i++)
        delete bubbles[i];

    for (int i = 0; i < tabOfPhoto.length(); i++)
        delete tabOfPhoto[i];

    delete sliderForImage;

    delete page;
}

void SubWin::save ()
{
    QColor color(Qt::white);
    page->fill(color);
    labPage->setPixmap(QPixmap::fromImage(*page));

    m_painter->begin(page);
    m_painter->drawImage(activePhoto->x(), activePhoto->y(), activePhoto->finalImage());
    for (int i = 0; i < tabOfPhoto.length(); i++)
        m_painter->drawImage(tabOfPhoto[i]->x(), tabOfPhoto[i]->y(), tabOfPhoto[i]->finalImage());
    for (int i = 0; i < bubbles.length(); i++)
        m_painter->drawImage(bubbles[i]->x(), bubbles[i]->y(), bubbles[i]->createFinalImage());
    m_painter->end();
    QString fname = QFileDialog::getSaveFileName(this, nullptr, nullptr, ".png");
    if (fname != nullptr)
        page->save(fname, "PNG");
}

void SubWin::reverseH ()
{
    if (tabOfPhoto.length() > 0)
        activePhoto->reverseH();
}

void SubWin::reverseV ()
{
    if (tabOfPhoto.length() > 0)
        activePhoto->reverseV();
}

void SubWin::moveElement (QMouseEvent* event, QPoint relativePos)
{
    QPoint point = mapFromGlobal(event->globalPos());

    movingBubble = qobject_cast<Bubble*>(sender());

    movingBubble->move(point.x() - relativePos.x() , point.y() - relativePos.y());
}

void SubWin::movePhoto (QMouseEvent* event, QPoint relativePos)
{
    activePhoto = qobject_cast<Photo*>(sender());

    QPoint point = mapFromGlobal(event->globalPos());

    if (activePhoto != nullptr)
    {
        activePhoto->move(point.x() - relativePos.x(), point.y() - relativePos.y());
    }

    sliderForImage->move(activePhoto->x(), activePhoto->y());
    sliderForImage->setValue((activePhoto->width() - MIN_IMG_SIZE_W)/35);
    sliderForImage->raise();
    sliderForImage->show();
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
    Photo* p = new Photo(this);
    p->loadImage();
    tabOfPhoto.push_back(p);
    activePhoto = p;
    sliderForImage->setValue((p->width() - MIN_IMG_SIZE_W)/35);
    connect(activePhoto, SIGNAL(grabbed(QMouseEvent*, QPoint)), this, SLOT(movePhoto(QMouseEvent*, QPoint)));
}

void SubWin::mouseDoubleClickEvent (QMouseEvent*)
{
    if (editingBubble != nullptr)
        editingBubble->setInactive();

    sliderForImage->hide();
}

void SubWin::updateEditingBubble()
{
    editingBubble = qobject_cast<Bubble*>(sender());
}

void SubWin::resizeWidth(int value)
{
    //activePhoto->resize(value);
}

void SubWin::crop ()
{
    if (tabOfPhoto.length() > 0)
    {
        activePhoto->crop();
        sliderForImage->setValue((activePhoto->width() - MIN_IMG_SIZE_W)/35);
    }
}
