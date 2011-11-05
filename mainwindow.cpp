#include <QSqlQuery>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opmlimport.h"
#include "podcastsdb.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    rfModel(this)
{
    ui->setupUi(this);
    connect(ui->actionImport, SIGNAL(triggered()), this, SLOT(testImport()));
    connect(ui->actionUpdateAll, SIGNAL(triggered()), &pdb, SLOT(updateAll()));

    ui->RSSView->setModel(&rfModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::testImport ()
{
    OPMLImport ("feeds01.opml");
}
