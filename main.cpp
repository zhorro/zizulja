#include <QtGui/QApplication>
#include "mainwindow.h"
#include "podcastsdb.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pdb.start();
    pdb.wait();

    MainWindow w;
    w.show();


    return a.exec();
}
