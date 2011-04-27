/*
 *	qt:paragraph:ui:MainWindow.cc
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2011
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

// Self-include:
#include "MainWindow.hh"

// Internal includes:
#include "misc/Settings.hh"
#include "ui/WorkingArea.hh"
#include "Context.hh"
#include "ui/ui_Actions.hh"

// Ku2 includes:
#include <ku2/debug.h>

// External includes:
#include <QAction>
#include <QToolBar>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>

using namespace ku2::paragraph;
using namespace ku2::paragraph::ui;

MainWindow::MainWindow():
		m_workingArea(new WorkingArea())
{
	restoreWindow();
	createActions();
	createMenus();
	createToolBars();

	Context *context = Context::instance();
	connect(context, SIGNAL(notifyOpened(bool)),
	        m_workingArea, SLOT(onGraphOpened(bool)));

	setCentralWidget(m_workingArea);
}

void MainWindow::createActions()
{
	connect(action(ACT_NEW_GRAPH), SIGNAL(triggered()),
	        this, SLOT(onNewGraphAction()));
	connect(action(ACT_VISUALISE_GRAPH), SIGNAL(triggered()),
	        this, SLOT(onVisualiseGraphAction()));
	connect(action(ACT_EXIT_APP), SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::createMenus()
{
	QMenuBar *bar = menuBar();

	//% "Graph"
	//: Graph related top-level menu item.
	QMenu *graphMenu = bar->addMenu(qtTrId("qtn_graph_menu"));
	graphMenu->addAction(action(ACT_VISUALISE_GRAPH));
}

void MainWindow::createToolBars()
{
	QToolBar *mainToolBar = addToolBar("Main");
	mainToolBar->addAction(action(ACT_NEW_GRAPH));
	mainToolBar->addAction(action(ACT_OPEN_DOCUMENT));
	mainToolBar->addAction(action(ACT_SAVE_DOCUMENT));
	mainToolBar->addSeparator();
	mainToolBar->addAction(action(ACT_EXIT_APP));
}

void MainWindow::saveWindow()
{
	Settings settings;
	settings.setValue("geometry", saveGeometry());
	settings.setValue("windowState", saveState());
}

void MainWindow::restoreWindow()
{
	Settings settings;
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("windowState").toByteArray());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	saveWindow();
	QMainWindow::closeEvent(event);
}

void MainWindow::onNewGraphAction()
{
	pstart();
	Context *const context = Context::instance();
	if ( !context->newGraph() )
		context->showLastError();
	pstop();
}

void MainWindow::onVisualiseGraphAction()
{
	pstart();
	Context *const context = Context::instance();
	if ( !context->visualiseGraph() )
		context->showLastError();
	pstop();
}
