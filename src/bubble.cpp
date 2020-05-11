#include "bubble.h"

Bubble::Bubble(int ratio, QWidget* parent) :   QLabel (parent),
                                              topLeftGrip(this),
                                              topRightGrip(this),
                                              downLeftGrip(this),
                                              downRightGrip(this)
{
    // Create the painter object
    painter = new QPainter;

    // Draw the bubble image
    img = new QImage(MIN_SIZE_W, MIN_SIZE_H, QImage::Format_ARGB32);

    QColor color(Qt::transparent);
    img->fill(color);

    painter->begin(img);
    painter->setRenderHints(QPainter::Antialiasing, true);

    QPainterPath path(QPointF(0, 0));
    path.addEllipse(QRectF(10, 10, img->width()-20, img->height()-20));

    painter->drawPath(path);
    painter->fillPath(path, QBrush(Qt::white));
    painter->end();

    // Set up the editing text
    editingText = new QTextEdit(this);
    editingText->setGeometry(20, 20, img->width()-40, img->height()-40);
    editingText->setAlignment(Qt::AlignCenter);
    editingText->setFont(QFont("Comic Sans MS", DEFAULT_FONT_POINTSIZE*ratio/100));
    editingText->setVisible(false);

    // Set up the context Menu
    contextMenu = new QMenu(this);
    act_raise = new QAction("raise", this);
    contextMenu->addAction(act_raise);

    connect(act_raise, SIGNAL(triggered()), this, SLOT(raise()));

    // Set up the widget properties
    setWindowFlag(Qt::SubWindow);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(MIN_SIZE_W, MIN_SIZE_H);

    // Display the bubble image
    setPixmap(QPixmap::fromImage(*img));
    setGeometry(0, 0, img->width(), img->height());
    show();

    // Position the grips
    topLeftGrip.move(0, 0);
    topRightGrip.move(width()-topRightGrip.width(), 0);
    downLeftGrip.move(0, height()-downLeftGrip.height());
    downRightGrip.move(width()-downRightGrip.width(), height()-downRightGrip.height());
}

Bubble::~Bubble()
{
    delete painter;
    delete img;

    delete editingText;

    delete contextMenu;
    delete act_raise;
}

QImage Bubble::createFinalImage(int ratio)
{    
    // Create final Image object
    QImage finalImage (img->width()*100/ratio, img->height()*100/ratio, QImage::Format_ARGB32);

    // Fill it with transparent color
    QColor color(Qt::transparent);
    finalImage.fill(color);

    // Draw the white bubble
    painter->begin(&finalImage);
    painter->setRenderHints(QPainter::Antialiasing, true);

    QPainterPath path(QPointF(0, 0));
    path.addEllipse(QRectF(10*100/ratio, 10*100/ratio,
                           finalImage.width()-20*100/ratio, finalImage.height()-20*100/ratio));

    painter->setPen(Qt::black);
    painter->drawPath(path);
    painter->fillPath(path, QBrush(Qt::white));

    // Draw the text
    QFont newFont = editingText->font();
    newFont.setPointSize(newFont.pointSize()*100/ratio);

    painter->setFont(newFont);

    QTextOption textOp;
    textOp.setWrapMode(QTextOption::WordWrap);
    textOp.setAlignment(Qt::AlignCenter);

    painter->drawText(QRectF(editingText->x()*100/ratio, editingText->y()*100/ratio,
                             editingText->width()*100/ratio, editingText->height()*100/ratio),
                             editingText->toPlainText(), textOp);

    painter->end();

    return finalImage;
}

// Mouse events
void Bubble::mousePressEvent (QMouseEvent *event)
{
    clickPosistion = mapFromGlobal(event->globalPos());
}

/*
void Bubble::mouseReleaseEvent (QMouseEvent *event)
{
    event->setAccepted(true);

    editingText->setVisible(false);

    // Repaint the image
    // Draw the bubble
    QPainterPath path(QPointF(0, 0));
    path.addEllipse(QRectF(10, 10, img->width()-20, img->height()-20));

    painter->begin(img);
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->setPen(Qt::black);
    painter->drawPath(path);
    painter->fillPath(path, QBrush(Qt::white));

    // Draw the text
    painter->setFont(editingText->font());

    QTextOption textOp;
    textOp.setWrapMode(QTextOption::WordWrap);
    textOp.setAlignment(Qt::AlignCenter);

    painter->drawText(QRectF(editingText->x(), editingText->y(),
                             editingText->width(), editingText->height()),
                             editingText->toPlainText(), textOp);

    painter->end();

    setPixmap(QPixmap::fromImage(*img));
}
*/

void Bubble::mouseMoveEvent (QMouseEvent *event)
{
    QPoint point = mapFromGlobal(event->globalPos());
    point = mapToParent(point);

    move(point.x() - clickPosistion.x() , point.y() - clickPosistion.y());
}

void Bubble::mouseDoubleClickEvent (QMouseEvent *event)
{
    event->setAccepted(true);

    if (editingText->isVisible() == false)
    {
        editingText->setVisible(true);
        grabMouse();
        editingText->grabKeyboard();
    }
    else
    {
        editingText->setVisible(false);

        // Repaint the image
        // Draw the bubble
        QPainterPath path(QPointF(0, 0));
        path.addEllipse(QRectF(10, 10, img->width()-20, img->height()-20));

        painter->begin(img);
        painter->setRenderHints(QPainter::Antialiasing, true);
        painter->setPen(Qt::black);
        painter->drawPath(path);
        painter->fillPath(path, QBrush(Qt::white));

        // Draw the text
        painter->setFont(editingText->font());

        QTextOption textOp;
        textOp.setWrapMode(QTextOption::WordWrap);
        textOp.setAlignment(Qt::AlignCenter);

        painter->drawText(QRectF(editingText->x(), editingText->y(),
                                 editingText->width(), editingText->height()),
                                 editingText->toPlainText(), textOp);

        painter->end();

        setPixmap(QPixmap::fromImage(*img));

        editingText->releaseKeyboard();
        releaseMouse();
    }
}

void Bubble::contextMenuEvent(QContextMenuEvent *event)
{
    contextMenu->move(event->globalPos());
    contextMenu->show();
}

// Resize
void Bubble::resize(int ratio)
{
    // Change the font pointsize
    QFont newFont = editingText->font();
    newFont.setPointSize(newFont.pointSize()*ratio/100);
    editingText->setFont(newFont);

    // Resize everything
    setGeometry(x()*ratio/100, y()*ratio/100, img->width()*ratio/100, img->height()*ratio/100);
}

void Bubble::resizeEvent (QResizeEvent*)
{
    // Repaint the image
    *img = QImage(this->width(), this->height(), QImage::Format_ARGB32);
    QColor color(Qt::transparent);
    img->fill(color);
    QPainterPath path(QPointF(0, 0));
    path.addEllipse(QRectF(10, 10, img->width()-20, img->height()-20));

    painter->begin(img);
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->setPen(Qt::black);
    painter->drawPath(path);
    painter->fillPath(path, QBrush(Qt::white));

    painter->setFont(editingText->font());

    QTextOption textOp;
    textOp.setWrapMode(QTextOption::WordWrap);
    textOp.setAlignment(Qt::AlignCenter);

    // Resize and reposition the text
    editingText->setGeometry(20, 20, img->width()-40, img->height()-40);

    // Draw the text
    painter->drawText(QRectF(editingText->x(), editingText->y(),
                             editingText->width(), editingText->height()),
                             editingText->toPlainText(), textOp);

    painter->end();

    setPixmap(QPixmap::fromImage(*img));

    // Reposition grips
    topLeftGrip.move(0, 0);
    topRightGrip.move(width()-topRightGrip.width(), 0);
    downLeftGrip.move(0, height()-downLeftGrip.height());
    downRightGrip.move(width()-downRightGrip.width(), height()-downRightGrip.height());
}

