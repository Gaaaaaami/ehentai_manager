
#include "GamiMainWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    GamiMainWidget w;
    w.show();
    return a.exec();
}

