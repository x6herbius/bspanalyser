#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pCommands(new MainWindowCommands(this)),
    m_pBSPFile(new BSPFile(this))
{
    ui->setupUi(this);

    ui->mainwidget->hexDisplayWidget()->setBSPFile(m_pBSPFile);

    connect(m_pBSPFile, &BSPFile::loaded, this, &MainWindow::bspFileLoaded);
    connect(m_pBSPFile, &BSPFile::cleared, this, &MainWindow::bspFileCleared);

    connect(ui->actionOpen, &QAction::triggered, m_pCommands, &MainWindowCommands::menuLoadFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

BSPFile* MainWindow::bspFile()
{
    return m_pBSPFile;
}

void MainWindow::bspFileLoaded()
{
    ui->mainwidget->hexDisplayWidget()->loadBSPData();
}

void MainWindow::bspFileCleared()
{
    ui->mainwidget->hexDisplayWidget()->clear();
}

void MainWindow::showTemporaryStatusMessage(const QString &message, int timeout)
{
    ui->statusBar->showMessage(message, timeout);
}
