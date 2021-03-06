/*
 *	qt:paragraph:ui:WorkingArea.cc
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
#include "WorkingArea.hh"

// Ku2 includes:
#include <ku2/debug.h>

// Internal includes:
#include "Context.hh"
#include "graph/graph_GraphModel.hh"

// External includes:
#include <QTableView>
#include <KugvGraph.hh>
#include <QGraphicsView>
#include <QDateTime>

using namespace ku2::paragraph::ui;

WorkingArea::WorkingArea():
		m_dummyWidget(NULL)
{
	setObjectName("WorkingArea");
	createDummyTab();
}

void WorkingArea::createDummyTab()
{
	m_dummyWidget = new QWidget(this);

	//% "(no tabs are currently open)"
	//: This is the name of the dummy tab, when no other tabs are open.
	addTab(m_dummyWidget, qtTrId("qtn_no_tabs_open"));
}

void WorkingArea::closeDummyTab()
{
	if ( m_dummyWidget ) {
		clear();
		delete m_dummyWidget;
	}
}

void WorkingArea::createGraphViewTab()
{
	m_graphView = new QTableView(this);
	m_graphView->setModel(Context::instance()->graphModel());
	m_graphView->resizeColumnsToContents();

	//% "Adjacency matrix"
	//: This is a name of the adjacency matrix tab.
	addTab(m_graphView, qtTrId("qtn_adj_matrix"));
}

void WorkingArea::createGraphVisualTab()
{
	pstart();

	QGraphicsView *view = new QGraphicsView(this);
	QGraphicsScene *scene = new QGraphicsScene(view);

	scene->addLine(0, 0, 100, 250);
	view->setScene(scene);

	//% "Visualisation (%1)"
	addTab(view, qtTrId("qtn_graph_visual")
	             .arg(QDateTime::currentDateTime().toString()));
	pstop();
}

void WorkingArea::onGraphOpened( bool opened )
{
	if ( opened ) {
		closeDummyTab();
		createGraphViewTab();
	} else {
		createDummyTab();
	}
}

void WorkingArea::onVisualise()
{
	createGraphVisualTab();
}
