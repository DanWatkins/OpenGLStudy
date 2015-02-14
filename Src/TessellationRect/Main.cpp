#include <QtGui/QGuiApplication>

#include "AppWindow.h"

int main(int argc, char **argv)
{
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    AppWindow window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

    return app.exec();
}