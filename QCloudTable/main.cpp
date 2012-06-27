#include <QtGui/QApplication>
#include "qcloudtable.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCloudTable w;
    w.show();
    
    return a.exec();
}
