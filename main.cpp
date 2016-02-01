#include "rip3d.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<Clowd>("Clowd");
    Rip3d w;
    w.show();

    return a.exec();
}
