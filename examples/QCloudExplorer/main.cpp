#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
//![1]
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//![1]
    return a.exec();
}
