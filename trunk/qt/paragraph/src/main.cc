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

// Ku2 includes:
#include <ku2/memory.h>
#include <ku2/debug.h>

// External includes:
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <exception>
#include <new>

using namespace ku2::paragraph;

void *operator new( size_t size )
{
	void *ptr = ku2::dmalloc(size);
	if ( ptr == NULL )
		throw std::bad_alloc();
	return ptr;
}

void operator delete( void *ptr )
{
	ku2::dfree(ptr);
}

int main( int argc, char *argv[] )
{
	ku2::func_debug_on;
	QApplication app(argc, argv);
	QTranslator translator;
	translator.load(":/translations/paragraph_" + QLocale::system().name());
	app.installTranslator(&translator);
	ui::MainWindow mainWindow;
	mainWindow.show();
	return app.exec();
}
