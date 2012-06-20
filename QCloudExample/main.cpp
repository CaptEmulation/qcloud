#include <QtGui/QApplication>
#include "cloudexample.h"
#include <QDir>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir::setCurrent("c:\temp");
    CloudExample w;
    w.show();
    
    return a.exec();
}
