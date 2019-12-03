#include "bubble.h"

Bubble::Bubble(QWidget* parent) : QLabel (parent)
{      
    img = new QImage(MIN_SIZE_W, MIN_SIZE_H, QImage::Format_ARGB32);
    QColor color(Qt::transparent);
    img->fill(color);
    painter = new QPainter(img);
    painter->setRenderHints(QPainter::Antialiasing, true);

    editingText = new QTextEdit(this);
    editingText->setGeometry(20, 20, img->width()-40, img->height()-40);
    editingText->setVisible(false);
    editingText->setAlignment(Qt::AlignCenter);

    printedText = new QLabel(this);
    printedText->setGeometry(20, 20, img->width()-40, img->height()-40);
    printedText->setAlignment(Qt::AlignCenter);
    printedText->setWordWrap(true);

    // Paint the bubble
    QPainterPath path(QPointF(0, 0));
    path.addEllipse(QRectF(10, 10, img->width()-20, img->height()-20));
    painter->drawPath(path);
    painter->fillPath(path, QBrush(Qt::white));

    // Print a text in the bubble
    QFont font("Comic Sans MS", 10);
    painter->setFont(font);
    //painter->drawText(QRectF(0, 0, img->width(), img->height()), Qt::AlignCenter, "VEUX-TU DES\n CHIPS, MON\n P'TIT CANARD ?");

    painter->end();

    setPixmap(QPixmap::fromImage(*img));
    setGeometry(0, 0, img->width(), img->height());
}

Bubble::~Bubble()
{
    delete painter;
    delete img;

    delete editingText;

    delete printedText;
}

void Bubble::mousePressEvent (QMouseEvent* event)
{
    relativePos = mapFromGlobal(event->globalPos());
}

void Bubble::mouseMoveEvent (QMouseEvent* event)
{
    emit grabbed(event, relativePos);
}

void Bubble::mouseDoubleClickEvent (QMouseEvent *)
{
    if (editingText->isVisible() == false)
    {
        emit editing();
        printedText->setVisible(false);
        editingText->setVisible(true);
        grabMouse();
        editingText->grabKeyboard();
    }
    else
    {
        editingText->setVisible(false);
        printedText->setVisible(true);
        printedText->setText(editingText->toPlainText());
        editingText->releaseKeyboard();
        releaseMouse();
    }
}

void Bubble::resizeWidth(int value)
{
    setGeometry(x(), y(), MIN_SIZE_W+10*value, height());

    // Repaint the image
    delete img;
    img = new QImage(this->width(), this->height(), QImage::Format_ARGB32);
    QColor color(Qt::transparent);
    img->fill(color);
    QPainterPath path(QPointF(0, 0));
    path.addEllipse(QRectF(10, 10, img->width()-20, img->height()-20));
    painter->begin(img);
    painter->setRenderHints(QPainter::Antialiasing, true);

    painter->setPen(Qt::black);
    painter->drawPath(path);
    painter->fillPath(path, QBrush(Qt::white));
    painter->end();

    editingText->setGeometry(20, 20, img->width()-40, img->height()-40);
    printedText->setGeometry(20, 20, img->width()-40, img->height()-40);
    setPixmap(QPixmap::fromImage(*img));
}

void Bubble::resizeHeight(int value)
{
    int newHeight = MIN_SIZE_H+10*value;
    int lastHeight = height();
    int changeInHeight = newHeight - lastHeight;
    setGeometry(x(), y()-changeInHeight, width(), newHeight);

    // Repaint the image
    delete img;
    img = new QImage(this->width(), this->height(), QImage::Format_ARGB32);
    QColor color(Qt::transparent);
    img->fill(color);
    QPainterPath path(QPointF(0, 0));
    path.addEllipse(QRectF(10, 10, img->width()-20, img->height()-20));
    painter->begin(img);
    painter->setRenderHints(QPainter::Antialiasing, true);

    painter->setPen(Qt::black);
    painter->drawPath(path);
    painter->fillPath(path, QBrush(Qt::white));
    painter->end();

    editingText->setGeometry(20, 20, img->width()-40, img->height()-40);
    printedText->setGeometry(20, 20, img->width()-40, img->height()-40);
    setPixmap(QPixmap::fromImage(*img));
}

QImage Bubble::createFinalImage()
{
    QTextOption textOp;
    textOp.setWrapMode(QTextOption::WordWrap);
    textOp.setAlignment(Qt::AlignCenter);
    QImage finalImage(*img);

    painter->begin(&finalImage);
    painter->drawText(QRectF(editingText->x(), editingText->y(), editingText->width(), editingText->height()), editingText->toPlainText(), textOp);
    painter->end();

    return finalImage;
}

void Bubble::setInactive ()
{
    editingText->setVisible(false);
    printedText->setVisible(true);
    printedText->setText(editingText->toPlainText());
    editingText->releaseKeyboard();
    releaseMouse();
}


