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
    menuEdit->addAction(act_newPhoto);

    act_reverseH = new QAction("Reverse horizontally");
    act_reverseH->setEnabled(false);
    menuEdit->addAction(act_reverseH);

    act_reverseV = new QAction("Reverse vertically");
    act_reverseV->setEnabled(false);
    menuEdit->addAction(act_reverseV);

    act_crop = new QAction("Crop");
    act_crop->setEnabled(false);
    menuEdit->addAction(act_crop);

    act_supprPhoto = new QAction("Suppress the photo");
    act_supprPhoto->setEnabled(false);
    menuEdit->addAction(act_supprPhoto);

    menuEdit->addSeparator();

    act_bubble = new QAction("Add a new bubble");
    menuEdit->addAction(act_bubble);

    menuBar()->addMenu(menuEdit);

    // Set up the View menu
    menuView = new QMenu("View");

    act_zoomIn = new QAction("Zoom +");
    menuView->addAction(act_zoomIn);

    act_zoomOut = new QAction("Zoom -");
    menuView->addAction(act_zoomOut);

    menuBar()->addMenu(menuView);

    // Set up the tool bar
    toolBar = new QToolBar(this);

    zoomSlider = new QSlider(Qt::Horizontal, this);
    zoomSlider->setMinimum(10);
    zoomSlider->setMaximum(200);
    zoomSlider->setValue(100);
    toolBar->addWidget(zoomSlider);

    addToolBar(toolBar);

    // Set up the central widget
    scroll = new QScrollArea(this);

    centralSubWin = new SubWin(scroll, this);
    scroll->setWidget(centralSubWin);

    setCentralWidget(scroll);

    // connections from actions to subWin
    connect(act_save, SIGNAL(triggered()), centralSubWin, SLOT(save()));
    connect(act_newPhoto, SIGNAL(triggered()), centralSubWin, SLOT(loadImage()));
    connect(act_reverseH, SIGNAL(triggered()), centralSubWin, SLOT(reverseH()));
    connect(act_reverseV, SIGNAL(triggered()), centralSubWin, SLOT(reverseV()));
    connect(act_crop, SIGNAL(triggered()), centralSubWin, SLOT(crop()));
    connect(act_supprPhoto, SIGNAL(triggered()), centralSubWin, SLOT(supprPhoto()));
    connect(act_bubble, SIGNAL(triggered()), centralSubWin, SLOT(addBubble()));
    connect(act_zoomIn, SIGNAL(triggered()), centralSubWin, SLOT(zoomIn()));
    connect(act_zoomOut, SIGNAL(triggered()), centralSubWin, SLOT(zoomOut()));

    // connections from toolBar widgets to SubWin
    connect(zoomSlider, SIGNAL(valueChanged(int)), centralSubWin, SLOT(resizePage(int)));

    // connections from subWin
    connect(centralSubWin, SIGNAL(containsImage(bool)), this, SLOT(activateActions(bool)));
    connect(centralSubWin, SIGNAL(hasASelectingPhoto(bool)), this, SLOT(activateCropAction(bool)));

    // Set geometry of the window
    setGeometry(0, 0, 800, 600);
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
    delete act_supprPhoto;
    delete act_bubble;
    delete menuEdit;

    // Menu View
    delete act_zoomIn;
    delete act_zoomOut;
    delete menuView;

    // Tool Bar
    delete zoomSlider;
    delete toolBar;

    // central widget
    delete centralSubWin;
    delete scroll;
}

void Window::activateActions(bool activating)
{
    if (activating)
    {
        act_reverseH->setEnabled(true);
        act_reverseV->setEnabled(true);
        act_supprPhoto->setEnabled(true);
    }
    else
    {
        act_reverseH->setEnabled(false);
        act_reverseV->setEnabled(false);
        act_crop->setEnabled(false);
        act_supprPhoto->setEnabled(false);
    }
}

void Window::activateCropAction(bool selecting)
{
    act_crop->setEnabled(selecting);
}
