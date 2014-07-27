#include <QApplication>
#include "triangletest.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QSurfaceFormat format;
    format.setSamples(16);

    TriangleWindow window;
    window.setFormat(format);
    window.resize(800, 600);
    window.show();

    window.setAnimating(true);

    return app.exec();
}
