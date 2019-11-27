#include "bubble.h"

Bubble::Bubble(QWidget* parent) : QLabel (parent)
{
    img = new QImage(140, 100, QImage::Format_ARGB32);
    QColor color(Qt::transparent);
    img->fill(color);
    painter = new QPainter(img);

    text = new QTextEdit(QString("..."));

    // Paint the bubble
    QPainterPath path(QPointF(0, 0));
    path.addEllipse(QRectF(0, 0, img->width(), img->height()));
    painter->drawPath(path);
    painter->fillPath(path, QBrush(Qt::white));

    // Print a text in the bubble
    QFont font("Comic Sans MS", 10);
    painter->setFont(font);
    painter->drawText(QRectF(0, 0, img->width(), img->height()), Qt::AlignCenter, "VEUX-TU DES\n CHIPS, MON\n P'TIT CANARD ?");

    painter->end();

    setPixmap(QPixmap::fromImage(*img));
    setGeometry(0, 0, img->width(), img->height());
}

Bubble::~Bubble()
{
    delete painter;
    delete img;

    delete text;
}

void Bubble::mousePressEvent (QMouseEvent* event)
{
    relativePos = mapFromGlobal(event->globalPos());
}

void Bubble::mouseMoveEvent (QMouseEvent* event)
{
    emit grabbed(event->globalPos());
}

