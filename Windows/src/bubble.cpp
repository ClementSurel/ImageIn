#include "bubble.h"

Bubble::Bubble(QWidget* parent) :   QLabel (parent),
                                    topLeftGrip(this),
                                    topRightGrip(this),
                                    downLeftGrip(this),
                                    downRightGrip(this)
{      
    // Create the bubble image
    img = new QImage(MIN_SIZE_W, MIN_SIZE_H, QImage::Format_ARGB32);
    QColor color(Qt::transparent);
    img->fill(color);

    painter = new QPainter(img);
    painter->setRenderHints(QPainter::Antialiasing, true);

    QPainterPath path(QPointF(0, 0));
    path.addEllipse(QRectF(10, 10, img->width()-20, img->height()-20));
    painter->drawPath(path);
    painter->fillPath(path, QBrush(Qt::white));

    // Set up the textEdit
    editingText = new QTextEdit(this);
    editingText->setGeometry(20, 20, img->width()-40, img->height()-40);
    editingText->setVisible(false);
    editingText->setAlignment(Qt::AlignCenter);

    printedText = new QLabel(this);
    printedText->setGeometry(20, 20, img->width()-40, img->height()-40);
    printedText->setAlignment(Qt::AlignCenter);
    printedText->setWordWrap(true);

    // Set up the text format in the bubble
    QFont font("Comic Sans MS", 12);
    painter->setFont(font);
    painter->end();

    // Context Menu
    contextMenu = new QMenu(this);
    act_raise = new QAction("raise", this);
    contextMenu->addAction(act_raise);

    connect(act_raise, SIGNAL(triggered()), this, SLOT(raise()));

    // Set up widget properties
    setWindowFlag(Qt::SubWindow);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(60, 60);

    // Display bubble image
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
    delete printedText;

    delete contextMenu;
    delete act_raise;
}

void Bubble::setInactive ()
{
    editingText->setVisible(false);
    printedText->setVisible(true);
    printedText->setText(editingText->toPlainText());
    editingText->releaseKeyboard();
    releaseMouse();
}

QImage Bubble::createFinalImage()
{
    QTextOption textOp;
    textOp.setWrapMode(QTextOption::WordWrap);
    textOp.setAlignment(Qt::AlignCenter);
    QImage finalImage(*img);

    painter->begin(&finalImage);
    painter->drawText(QRectF(editingText->x(), editingText->y(), editingText->width(), editingText->height()),
                      editingText->toPlainText(), textOp);
    painter->end();

    return finalImage;
}

// Events
void Bubble::mousePressEvent (QMouseEvent* event)
{
    relativePos = mapFromGlobal(event->globalPos());
}

void Bubble::mouseMoveEvent (QMouseEvent* event)
{
    QPoint point = mapFromGlobal(event->globalPos());
    point = mapToParent(point);

    move(point.x() - relativePos.x() , point.y() - relativePos.y());
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

void Bubble::contextMenuEvent(QContextMenuEvent *event)
{
    contextMenu->move(event->globalPos());
    contextMenu->show();
}

void Bubble::resizeEvent (QResizeEvent*)
{
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

    // Reposition grips
    topLeftGrip.move(0, 0);
    topRightGrip.move(width()-topRightGrip.width(), 0);
    downLeftGrip.move(0, height()-downLeftGrip.height());
    downRightGrip.move(width()-downRightGrip.width(), height()-downRightGrip.height());
}
