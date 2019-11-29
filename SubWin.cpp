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

    //m_img = new QImage;
    m_img = new QImage(800, 600, QImage::Format_ARGB32);
    QColor color(Qt::transparent);
    m_img->fill(color);

    m_painter = new QPainter;

    m_lab = new QLabel(this);

    m_lab->setPixmap(QPixmap::fromImage(*m_img));

    m_lab->setGeometry(0, 0, m_img->width(), m_img->height());
    setGeometry(0, 0, 800, 600);

    activeBubble = nullptr;
}

SubWin::~SubWin()
{
    delete m_painter;
    delete m_img;
    delete m_lab;

    delete hSlider;

    for (int i = 0; i < bubbles.length(); i++)
        delete bubbles[i];
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
}

void SubWin::reverseV ()
{
    QImage *timg = new QImage;
    *timg = m_img->mirrored(false, true);
    delete m_img;
    m_img = timg;
    m_lab->setPixmap(QPixmap::fromImage(*m_img));
}

void SubWin::moveElement (QPoint globalPoint)
{
    QPoint point = mapFromGlobal(globalPoint);

    if (activeBubble != nullptr)
    {
        disconnect(hSlider, SIGNAL(valueChanged(int)), activeBubble, SLOT(resizeWidth(int)));
        disconnect(vSlider, SIGNAL(valueChanged(int)), activeBubble, SLOT(resizeHeight(int)));
    }

    activeBubble = qobject_cast<Bubble*>(sender());

    activeBubble->move(point.x() - activeBubble->width()/2, point.y() - activeBubble->height()/2);

    hSlider->move(activeBubble->x(), activeBubble->y()+activeBubble->height());
    hSlider->setValue((activeBubble->width()-MIN_SIZE_W)/10);
    hSlider->raise();
    hSlider->show();
    connect(hSlider, SIGNAL(valueChanged(int)), activeBubble, SLOT(resizeWidth(int)));

    vSlider->move(activeBubble->x()-vSlider->width(), activeBubble->y()+activeBubble->height()-vSlider->height());
    vSlider->setValue((activeBubble->height()-MIN_SIZE_H)/10);
    vSlider->raise();
    vSlider->show();
    connect(vSlider, SIGNAL(valueChanged(int)), activeBubble, SLOT(resizeHeight(int)));
}

void SubWin::addBubble()
{
    Bubble* newBubble = new Bubble(this);
    bubbles.push_back(newBubble);

    newBubble->show();

    connect(newBubble, SIGNAL(grabbed(QPoint)), this, SLOT(moveElement(QPoint)));
}

void SubWin::loadImage()
{
    QString fname = QFileDialog::getOpenFileName(this);

    if (fname != nullptr)
    {
        m_img->load(fname);

        QImage* newImg = new QImage;
        *newImg = m_img->scaled(QSize(800, 600), Qt::KeepAspectRatio);
        delete m_img;
        m_img = newImg;

        m_lab->setPixmap(QPixmap::fromImage(*m_img));
        m_lab->setGeometry(80, 80, m_img->width(), m_img->height());
    }
}
