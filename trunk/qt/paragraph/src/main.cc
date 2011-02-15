/*
 *	qt:paragraph:main.cc
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2011
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

// Internal includes:
#include "ui/MainWindow.hh"

// External includes:
#include <QApplication>
#include <QTranslator>
#include <QLocale>

using namespace ku2::paragraph;

int main( int argc, char *argv[] )
{
	QApplication app(argc, argv);
	QTranslator translator;
	translator.load(":/translations/paragraph_" + QLocale::system().name());
	app.installTranslator(&translator);
	ui::MainWindow mainWindow;
	mainWindow.show();
	return app.exec();
}
