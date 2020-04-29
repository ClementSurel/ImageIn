#ifndef FINALIMAGE_H
#define FINALIMAGE_H

#include <QtWidgets>

class FinalImage : public QLabel
{
    Q_OBJECT

public:
    FinalImage();
    ~FinalImage();
    QImage* getImg();

public slots:
    void reverseH();
    void reverseV();
    void save();

protected:
    void paintEvent(QPaintEvent*);

private:
    QImage* img;
};

#endif // FINALIMAGE_H
