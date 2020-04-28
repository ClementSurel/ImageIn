#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets>
#include <QImage>

#include "SubWin.h"

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window();

private:
    // Menu File
    QMenu* menuFile;
    QAction* act_save;

    // Menu Edit
    QMenu* menuEdit;
    QAction *act_newPhoto;
    QAction* act_reverseH;
    QAction* act_reverseV;
    QAction *act_crop;
    QAction* act_bubble;

    // Central Widget
    QScrollArea *scroll;
    SubWin* centralSubWin;
};

#endif // WINDOW_H
