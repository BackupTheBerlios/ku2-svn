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

#include <QStringList>
#include "graphviz/graph.hh"

using namespace ku2::paragraph::ui;

WorkingArea::WorkingArea()
{
	setObjectName("WorkingArea");
	createDummyTab();

	ku2::graphviz::Graph graph("Unnamed");
	graph.initialise();
	graph.addNodes(QStringList() << "1" << "2" << "3" << "4" << "5");
	graph.addEdge("1", "2");
	graph.addEdge("2", "3");
	graph.addEdge("2", "4");
	graph.addEdge("3", "5");
}

void WorkingArea::createDummyTab()
{
	//% "(no tabs are currently open)"
	//: This is the name of the dummy tab, when no other tabs are open.
	addTab(new QWidget(), qtTrId("qtn_no_tabs_open"));
}
