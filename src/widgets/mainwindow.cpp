#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pCommands(new MainWindowCommands(this)),
    m_pApplicationModel(new ApplicationModel(this))
{
    ui->setupUi(this);

    ui->mainwidget->hexDisplayWidget()->setBSPFile(m_pApplicationModel->bspFileModel());

    connect(m_pCommands, &MainWindowCommands::newFileLoaded, this, &MainWindow::bspFileLoaded);
    connect(ui->actionOpen, &QAction::triggered, m_pCommands, &MainWindowCommands::menuLoadFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

ApplicationModel* MainWindow::applicationModel()
{
    return m_pApplicationModel;
}

void MainWindow::bspFileLoaded()
{
    ui->mainwidget->hexDisplayWidget()->loadBSPData();
    ui->mainwidget->lumpViewArea()->updateLumps(m_pApplicationModel->bspFileStructure());
}

void MainWindow::showTemporaryStatusMessage(const QString &message, int timeout)
{
    ui->statusBar->showMessage(message, timeout);
}
