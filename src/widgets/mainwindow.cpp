#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pCommands(new MainWindowCommands(this)),
    m_pApplicationModel(new ApplicationModel(this))
{
    ui->setupUi(this);

    connect(m_pCommands, &MainWindowCommands::newFileLoaded, this, &MainWindow::bspFileUpdated);
    connect(m_pCommands, &MainWindowCommands::fileCleared, this, &MainWindow::bspFileUpdated);

    connect(ui->actionOpen, &QAction::triggered, m_pCommands, &MainWindowCommands::menuLoadFile);
    connect(ui->actionQuit, &QAction::triggered, qApp, &QCoreApplication::quit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

ApplicationModel* MainWindow::applicationModel()
{
    return m_pApplicationModel;
}

void MainWindow::bspFileUpdated()
{
    ui->mainwidget->updateLumps();
}

void MainWindow::showTemporaryStatusMessage(const QString &message, int timeout)
{
    ui->statusBar->showMessage(message, timeout);
}
