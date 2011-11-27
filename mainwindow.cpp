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

    connect(ui->pbCheckForUpdate1, SIGNAL(clicked()), &pdb, SLOT(updateAll()));
    connect(ui->pbCheckForUpdate2, SIGNAL(clicked()), &pdb, SLOT(updateAll()));

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
