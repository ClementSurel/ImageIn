#include <QPaintDevice>
#include <QPainter>
#include <QtWidgets>

#include "Window.h"

Window::Window(QWidget *parent) : QMainWindow(parent)
{

    // Menu File
    menuFile = new QMenu("File");
    m_act_save = new QAction("Save");

    menuFile->addSeparator();
    menuFile->addAction(m_act_save);
    menuBar()->addMenu(menuFile);

    // Menu Edit
    act_newPhoto = new QAction("Add a new photo");
    m_menuEdit = new QMenu("Edit");
    m_act_reverseH = new QAction("Reverse horizontally");
    m_act_reverseV = new QAction("Reverse vertically");
    m_bubble = new QAction("Add a new bubble");
    act_crop = new QAction("Crop");

    m_menuEdit->addAction(act_newPhoto);
    m_menuEdit->addAction(m_act_reverseH);
    m_menuEdit->addAction(m_act_reverseV);
    m_menuEdit->addAction(act_crop);
    m_menuEdit->addSeparator();
    m_menuEdit->addAction(m_bubble);
    menuBar()->addMenu(m_menuEdit);

    m_centralSubWin = new SubWin(this);
    c = new QScrollArea(this);
    c->setWidget(m_centralSubWin);

    setCentralWidget(c);
    setGeometry(0, 0, 800, 600);

    connect(m_act_reverseH, SIGNAL(triggered()), m_centralSubWin, SLOT(reverseH()));
    connect(m_act_reverseV, SIGNAL(triggered()), m_centralSubWin, SLOT(reverseV()));
    connect(m_act_save, SIGNAL(triggered()), m_centralSubWin, SLOT(save()));
    connect(m_bubble, SIGNAL(triggered()), m_centralSubWin, SLOT(addBubble()));
    connect(act_newPhoto, SIGNAL(triggered()), m_centralSubWin, SLOT(loadImage()));
    connect(act_crop, SIGNAL(triggered()), m_centralSubWin, SLOT(crop()));
}

Window::~Window()
{
    delete act_newPhoto;

    delete m_bubble;
    delete m_act_reverseH;
    delete m_act_reverseV;

    delete m_act_save;
    delete menuFile;

    delete m_menuEdit;

    delete m_centralSubWin;

    delete c;
}



