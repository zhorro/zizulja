#include <QtGui/QApplication>
#include "mainwindow.h"
#include "podcastsdb.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    pdb.start();

    return a.exec();
}
