#include "bubble.h"

Bubble::Bubble(QWidget* parent) : QLabel (parent)
{
    img = new QImage(160, 100, QImage::Format_ARGB32);
    QColor color(Qt::transparent);
    img->fill(color);
    painter = new QPainter(img);

    text = new QTextEdit(this);
    text->setGeometry(20, 20, img->width()-40, 60);
    //text->setVisible(false);
    text->setAlignment(Qt::AlignCenter);

    editingText = new QLabel(this);
    editingText->setGeometry(text->x(), text->y(), text->width(), text->height());
    editingText->setAlignment(Qt::AlignHCenter);
    editingText->setWordWrap(true);

    // Paint the bubble
    QPainterPath path(QPointF(0, 0));
    path.addEllipse(QRectF(0, 0, img->width(), img->height()));
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
    grabMouse();
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

void Bubble::focusOutEvent (QFocusEvent*)
{
    /*
    painter->begin(img);
    painter->drawText(QRectF(0, 0, img->width(), img->height()), Qt::AlignCenter, text->toPlainText());
    painter->end();
    */
    //text->hide();
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
