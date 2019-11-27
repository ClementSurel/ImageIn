#include <QPaintDevice>
#include <QPainter>
#include <QtWidgets>

#include "Window.h"

Window::Window(QWidget *parent) : QMainWindow(parent)
{
    m_menuFile = new QMenu("File");
    m_act_save = new QAction("Save");

    m_menuEdit = new QMenu("Edit");

    m_act_reverseH = new QAction("Reverse horizontally");
    m_act_reverseV = new QAction("Reverse vertically");

    m_bubble = new QAction("Bubble");

    m_centralSubWin = new SubWin;

    // menu File
    m_menuFile->addAction(m_act_save);
    menuBar()->addMenu(m_menuFile);

    // meun Edit
    m_menuEdit->addAction(m_act_reverseH);
    m_menuEdit->addAction(m_act_reverseV);
    m_menuEdit->addAction(m_bubble);
    menuBar()->addMenu(m_menuEdit);

    setCentralWidget(m_centralSubWin);

    connect(m_act_reverseH, SIGNAL(triggered()), m_centralSubWin, SLOT(reverseH()));
    connect(m_act_reverseV, SIGNAL(triggered()), m_centralSubWin, SLOT(reverseV()));
    connect(m_act_save, SIGNAL(triggered()), m_centralSubWin, SLOT(save()));
    connect(m_bubble, SIGNAL(triggered()), this, SLOT(update()));
}

Window::~Window()
{
    delete m_bubble;

    delete m_act_save;
    delete m_menuFile;

    delete m_menuEdit;

    delete m_centralSubWin;
}



