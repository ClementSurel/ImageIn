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
    // Menu File
    QMenu* m_menuFile;
    QAction* m_act_save;
    QAction* act_load;

    // Menu Edit
    QMenu* m_menuEdit;
    QAction* m_act_reverseH;
    QAction* m_act_reverseV;
    QAction* m_bubble;

    // central widget
    SubWin* m_centralSubWin;
};

#endif // WINDOW_H
