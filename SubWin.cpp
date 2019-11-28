#include <QDialog>

#include "SubWin.h"

SubWin::SubWin()
{
    m_img = new QImage;
    m_img->load("ex_laugh.jpg");

    QImage* newImg = new QImage;
    *newImg = m_img->scaled(QSize(600, 400), Qt::KeepAspectRatio);
    delete m_img;
    m_img = newImg;

    m_painter = new QPainter;

    m_lab = new QLabel(this);

    setGeometry(0, 0, 500, 500);

    m_lab->setPixmap(QPixmap::fromImage(*m_img));

    m_lab->setGeometry(0, 0, m_img->width(), m_img->height());
}

SubWin::~SubWin()
{
    delete m_painter;
    delete m_img;
    delete m_lab;

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
    QString fname = QFileDialog::getSaveFileName(this, nullptr, nullptr, ".bmp");
    imgToSave.save(fname, "BMP");
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

void SubWin::moveElement (QPoint globalPos)
{
    QPoint point = mapFromGlobal(globalPos);
    Bubble* pB = qobject_cast<Bubble*>(sender());

    pB->move(point.x() - pB->width()/2, point.y() - pB->height()/2);
}

void SubWin::addBubble()
{
    Bubble* newBubble = new Bubble(this);
    bubbles.push_back(newBubble);

    newBubble->show();

    connect(newBubble, SIGNAL(grabbed(QPoint)), this, SLOT(moveElement(QPoint)));
}

