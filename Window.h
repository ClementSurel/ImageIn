#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets>
#include <QMainWindow>
#include <QImage>
#include <QPixmap>

#include "SubWin.h"

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window();

private:
    QScrollArea *c;
    // Menu File
    QMenu* menuFile;
    QAction* m_act_save;

    // Menu Edit
    QMenu* m_menuEdit;
    QAction *act_newPhoto;
    QAction* m_act_reverseH;
    QAction* m_act_reverseV;
    QAction* m_bubble;
    QAction *act_crop;

    // central widget
    SubWin* m_centralSubWin;
};

#endif // WINDOW_H
