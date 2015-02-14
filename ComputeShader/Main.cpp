#include <QtGui/QGuiApplication>

#include "ComputeShader.h"

int main(int argc, char **argv)
{
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    ComputeShader window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

    return app.exec();
}