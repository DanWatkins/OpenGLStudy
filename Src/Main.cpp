//==================================================================================================================|
// Created 2014.07.26 by Daniel L. Watkins
//
// Copyright (C) 2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#include <QtWidgets\QApplication>
#include "MainWindow.h"

#if _MSC_VER >= 1310
	#ifdef _DEBUG
		#pragma comment (lib, "Qt5Guid.lib")
		#pragma comment (lib, "Qt5Cored.lib")
	#else
		#pragma comment (lib, "Qt5Gui.lib")
		#pragma comment (lib, "Qt5Core.lib")
	#endif
#endif


int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	QSurfaceFormat format;
	format.setSamples(16);

	MainWindow window;
	window.setFormat(format);
	window.resize(800, 600);
	window.show();

	window.setAnimating(true);

	return app.exec();
}