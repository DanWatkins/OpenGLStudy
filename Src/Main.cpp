//==================================================================================================================|
// Created 2013.07.26 by Daniel L. Watkins
//
// Copyright (C) 2013-2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#include <QApplication>
#include "MainWindow.h"

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
