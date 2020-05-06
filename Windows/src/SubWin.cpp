#include <QDialog>

#include "SubWin.h"

SubWin::SubWin(QScrollArea* givenScroll, QWidget* parent) : QWidget(parent),
                                                            scroll(givenScroll),
                                                            zoomRatio(100)
{
    // Page
    labPage = new QLabel(this);
    setGeometry(0, 0, PAGE_W, PAGE_H);

    page = new QImage(PAGE_W, PAGE_H, QImage::Format_ARGB32);
    QColor color(Qt::lightGray);
    page->fill(color);
    labPage->setPixmap(QPixmap::fromImage(*page));
    // Fill with white for save function
    color = QColor(Qt::white);
    page->fill(color);

    // Painter
    m_painter = new QPainter;

    // pointer to photo and bubble
    movingBubble = nullptr;
    editingBubble = nullptr;
    activePhoto = nullptr;
}

SubWin::~SubWin()
{
    if (m_painter != nullptr)
        delete m_painter;

    for (int i = 0; i < bubbles.length(); i++)
        delete bubbles[i];

    for (int i = 0; i < tabOfPhoto.length(); i++)
        delete tabOfPhoto[i];

    delete page;
    delete labPage;
}

void SubWin::mouseDoubleClickEvent (QMouseEvent*)
{
    if (editingBubble != nullptr)
        editingBubble->setInactive();
}

void SubWin::loadImage()
{
    Photo* p = new Photo(this);
    if (p->loadImage(zoomRatio))
    {
        p->move(scroll->horizontalScrollBar()->value(), scroll->verticalScrollBar()->value());
        tabOfPhoto.push_back(p);
        activePhoto = p;
        emit containsImage(true);
    }
    else
    {
        delete p;
    }
}

void SubWin::reverseH ()
{
    if (activePhoto != nullptr)
        activePhoto->reverseH();
}

void SubWin::reverseV ()
{
    if (activePhoto != nullptr)
        activePhoto->reverseV();
}

void SubWin::crop ()
{
    if (activePhoto != nullptr)
        activePhoto->crop();
}

void SubWin::supprPhoto ()
{
    for (int i = 0; i < tabOfPhoto.length(); i++)
        if (activePhoto == tabOfPhoto[i])
        {
            tabOfPhoto.remove(i);
            activePhoto->deleteLater();
            if (tabOfPhoto.length())
            {
                activePhoto = tabOfPhoto[tabOfPhoto.length()-1];
            }
            else
            {
                activePhoto = nullptr;
                emit containsImage(false);
            }
            break;
        }
}

void SubWin::addBubble()
{
    Bubble* newBubble = new Bubble(this);
    newBubble->move(scroll->horizontalScrollBar()->value(), scroll->verticalScrollBar()->value());

    bubbles.push_back(newBubble);

    newBubble->show();

    connect(newBubble, SIGNAL(editing()), this, SLOT(updateEditingBubble()));
}

void SubWin::save ()
{
    // Create the saved image. Draw all the photos and bubbles
    m_painter->begin(page);
    for (int i = 0; i < tabOfPhoto.length(); i++)
        m_painter->drawImage(tabOfPhoto[i]->x()*100/zoomRatio, tabOfPhoto[i]->y()*100/zoomRatio, tabOfPhoto[i]->finalImage(zoomRatio));
    for (int i = 0; i < bubbles.length(); i++)
        m_painter->drawImage(bubbles[i]->x(), bubbles[i]->y(), bubbles[i]->createFinalImage(zoomRatio));
    m_painter->end();

    // Ask user where to save the image
    QString fname = QFileDialog::getSaveFileName(this, nullptr, nullptr, ".png");
    if (fname != nullptr)
    {
        fname += ".png";
        page->save(fname, "PNG");
    }
}

void SubWin::updateEditingBubble()
{
    editingBubble = qobject_cast<Bubble*>(sender());
}

void SubWin::updateActivePhoto()
{
    activePhoto = qobject_cast<Photo*>(sender());
}

void SubWin::updateSelectingPhoto(bool selecting)
{
    emit hasASelectingPhoto(selecting);
}

// Zoom

void SubWin::zoomIn ()
{
    resizePage(true);
}

void SubWin::zoomOut()
{
    resizePage(false);
}

void SubWin::resizePage(bool zoomIn)
{
    int oldRatio = zoomRatio;

    if (zoomIn)
        zoomRatio += 10;
    else
        zoomRatio -= 10;

    if (zoomRatio <= 0)
    {
        zoomRatio = oldRatio;
        return;
    }

    setGeometry(0, 0, PAGE_W*zoomRatio/100, PAGE_H*zoomRatio/100);

    for (int i = 0; i < tabOfPhoto.size(); i++)
        tabOfPhoto[i]->resizeWithZoom(zoomRatio*100/oldRatio);
    for (int i = 0; i < bubbles.size(); i++)
        bubbles[i]->resize(zoomRatio*100/oldRatio);
}

