#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void testImport ();
private:
    Ui::MainWindow *ui;
};

//class Singleton
//{
//public:
//	static Singleton *instance()
//	{
//		static Singleton pointer;
//		return &pointer;
//	}
//private:
//	Singleton();
//};
//inline singleton()
//{
//	return Singleton::instance();
//}

#endif // MAINWINDOW_H
