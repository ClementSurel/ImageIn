#include <QPaintDevice>
#include <QPainter>
#include <QtWidgets>

#include "Window.h"

Window::Window(QWidget *parent) : QMainWindow(parent)
{
    // Set up File menu
    menuFile = new QMenu("File");
    act_save = new QAction("Save");

    menuFile->addAction(act_save);
    menuBar()->addMenu(menuFile);

    // Set up Edit menu
    menuEdit = new QMenu("Edit");
    act_newPhoto = new QAction("Add a new photo");
    act_reverseH = new QAction("Reverse horizontally");
    act_reverseV = new QAction("Reverse vertically");
    act_crop = new QAction("Crop");
    act_bubble = new QAction("Add a new bubble");

    menuEdit->addAction(act_newPhoto);
    menuEdit->addAction(act_reverseH);
    menuEdit->addAction(act_reverseV);
    menuEdit->addAction(act_crop);
    menuEdit->addSeparator();
    menuEdit->addAction(act_bubble);
    menuBar()->addMenu(menuEdit);

    // Set up the central widget
    centralSubWin = new SubWin(this);
    scroll = new QScrollArea(this);

    scroll->setWidget(centralSubWin);
    setCentralWidget(scroll);

    // Set geometry of the window
    setGeometry(0, 0, 800, 600);

    // connections from actions to central widget
    connect(act_save, SIGNAL(triggered()), centralSubWin, SLOT(save()));
    connect(act_newPhoto, SIGNAL(triggered()), centralSubWin, SLOT(loadImage()));
    connect(act_reverseH, SIGNAL(triggered()), centralSubWin, SLOT(reverseH()));
    connect(act_reverseV, SIGNAL(triggered()), centralSubWin, SLOT(reverseV()));
    connect(act_crop, SIGNAL(triggered()), centralSubWin, SLOT(crop()));
    connect(act_bubble, SIGNAL(triggered()), centralSubWin, SLOT(addBubble()));
}

Window::~Window()
{
    // Menu File
    delete act_save;
    delete menuFile;

    // Menu Edit
    delete act_newPhoto;
    delete act_reverseH;
    delete act_reverseV;
    delete act_crop;
    delete act_bubble;
    delete menuEdit;

    // central widget
    delete centralSubWin;
    delete scroll;
}



