#include "mainwindow.h"
#include "models/sharedinfo.h"
#include <QDebug>
#include <QQmlContext>
#include <omp.h>

MainWindow::MainWindow()
{
    mainInst = nullptr;
}

MainWindow::~MainWindow()
{
}

void MainWindow::close()
{
}

void MainWindow::objectCreated( QObject *object, const QUrl & )
{
    mainInst = object;
    rootContext()->setContextProperty( "mainWindowNSISGenerator", this );
}
