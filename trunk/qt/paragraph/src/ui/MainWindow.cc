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
#include "WorkingArea.hh"

// External includes:
#include <QAction>
#include <QToolBar>

using namespace ku2::paragraph::ui;

MainWindow::MainWindow():
		m_workingArea(new WorkingArea())
{
	restoreWindow();
	createActions();
	createMenus();
	createToolBars();

	setCentralWidget(m_workingArea);
}

void MainWindow::createActions()
{
	QList<QAction*> actions;
	actions << new QAction(QIcon(":/ui/graph-new.png"), "new-graph", this)
	        << new QAction(QIcon(":/ui/document-save.png"), "save-document", this)
	        << new QAction(QIcon(":/ui/document-open.png"), "open-document", this)
	        << new QAction(QIcon(":/ui/application-exit.png"), "exit-app", this);
	foreach ( QAction *action, actions )
		m_actions.insert(action->text(), action);
}

void MainWindow::createMenus()
{
}

void MainWindow::createToolBars()
{
	QToolBar *mainToolBar = addToolBar("Main");
	mainToolBar->addAction(m_actions.value("new-graph"));
	mainToolBar->addAction(m_actions.value("open-document"));
	mainToolBar->addAction(m_actions.value("save-document"));
	mainToolBar->addSeparator();
	mainToolBar->addAction(m_actions.value("exit-app"));
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
