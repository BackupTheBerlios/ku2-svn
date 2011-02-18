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

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>

using namespace ku2::paragraph::ui;

WorkingArea::WorkingArea()
{
	setObjectName("WorkingArea");
	createDummyTab();


}

void WorkingArea::createDummyTab()
{
	ku2::graphviz::Graph graph("Unnamed");
	graph.initialise();
	graph.addNodes(QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8");
	graph.addEdge("1", "2");
	graph.addEdge("2", "3");
	graph.addEdge("2", "4");
	graph.addEdge("3", "5");
	graph.addEdge("1", "5");
	graph.addEdge("5", "4");
	graph.addEdge("4", "6");
	graph.addEdge("6", "7");
	graph.addEdge("7", "8");
	graph.addEdge("8", "2");
	graph.setRootNode("1");
	graph.applyLayout(ku2::graphviz::Graph::LAYOUT_DOT);
	QList<ku2::graphviz::Node> nodes = graph.nodes();
	QList<ku2::graphviz::Edge> edges = graph.edges();

	QGraphicsView *view = new QGraphicsView();
	QGraphicsScene *scene = new QGraphicsScene();
	foreach ( const ku2::graphviz::Node &node, nodes ) {
		scene->addEllipse(node.centre().x()-5, node.centre().y()-5, 10, 10);
	}
	foreach ( const ku2::graphviz::Edge &edge, edges ) {
		scene->addPath(edge.path());
	}
	view->setScene(scene);

	//% "(no tabs are currently open)"
	//: This is the name of the dummy tab, when no other tabs are open.
	addTab(view, qtTrId("qtn_no_tabs_open"));
}
