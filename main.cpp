#include "rip3d.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSurfaceFormat format;
        format.setVersion(4, 3);
        format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
        format.setProfile(QSurfaceFormat::CoreProfile);
        format.setRenderableType(QSurfaceFormat::OpenGL);
        format.setDepthBufferSize(64);
        format.setStencilBufferSize(32);
        QSurfaceFormat::setDefaultFormat(format);
    qRegisterMetaType<Clowd>("Clowd");
    qRegisterMetaType<MathVector>("MathVector");
    qRegisterMetaType<IntVector>("IntVector");
    qRegisterMetaType<IntVector>("IntVector&");

    QCoreApplication::setOrganizationName("Nixson LLC");
    QCoreApplication::setOrganizationDomain("nixson.ru");
    QCoreApplication::setApplicationName("Rip3d");
    Rip3d w;
    w.show();

    return a.exec();
}
