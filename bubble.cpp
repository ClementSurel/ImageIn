#include "bubble.h"

Bubble::Bubble(QWidget* parent) : QLabel (parent)
{      
    img = new QImage(MIN_SIZE_W, MIN_SIZE_H, QImage::Format_ARGB32);
    QColor color(Qt::transparent);
    img->fill(color);
    painter = new QPainter(img);
    painter->setRenderHints(QPainter::Antialiasing, true);

    text = new QTextEdit(this);
    text->setGeometry(20, 20, img->width()-40, img->height()-40);
    text->setVisible(false);
    text->setAlignment(Qt::AlignCenter);

    editingText = new QLabel(this);
    editingText->setGeometry(20, 20, img->width()-40, img->height()-40);
    editingText->setAlignment(Qt::AlignCenter);
    editingText->setWordWrap(true);

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

    delete text;

    delete editingText;
}

void Bubble::mousePressEvent (QMouseEvent* event)
{
    relativePos = mapFromGlobal(event->globalPos());
}

void Bubble::mouseMoveEvent (QMouseEvent* event)
{
    emit grabbed(event->globalPos());
}

void Bubble::mouseDoubleClickEvent (QMouseEvent *)
{
    if (text->isVisible() == false)
    {
        editingText->setVisible(false);
        text->setVisible(true);
        grabMouse();
        text->grabKeyboard();
    }
    else
    {
        text->setVisible(false);
        editingText->setVisible(true);
        editingText->setText(text->toPlainText());
        text->releaseKeyboard();
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

    text->setGeometry(20, 20, img->width()-40, img->height()-40);
    editingText->setGeometry(20, 20, img->width()-40, img->height()-40);
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

    text->setGeometry(20, 20, img->width()-40, img->height()-40);
    editingText->setGeometry(20, 20, img->width()-40, img->height()-40);
    setPixmap(QPixmap::fromImage(*img));
}

QImage Bubble::createFinalImage()
{
    QTextOption textOp;
    textOp.setWrapMode(QTextOption::WordWrap);
    textOp.setAlignment(Qt::AlignCenter);
    QImage finalImage(*img);

    painter->begin(&finalImage);
    painter->drawText(QRectF(text->x(), text->y(), text->width(), text->height()), text->toPlainText(), textOp);
    painter->end();

    return finalImage;
}
