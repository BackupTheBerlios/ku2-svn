/*
 *	qt:paragraph:graph:Graph.cc
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
#include "graph_Graph.hh"

// External includes:
#include <KugvGraph.hh>

using namespace ku2;
using namespace ku2::paragraph::graph;

Graph::Graph( uint vertices ):
		m_graph(ku_graph_am_create(vertices, KUF_GRAPH_DEFAULT))
{
	pstartp("Previous call to ku_graph_am_create refers to here");
	pstop();
}

Graph::~Graph()
{
	pstart();
	if ( m_graph ) {
		ku_graph_am_free(m_graph);
	}
	pstop();
}

bool Graph::visualise( QGraphicsScene *scene ) const
{
	pstart();

	graphviz::Graph *graph = new graphviz::Graph("unnamed graph");
	for ( int i = 0; i < cntVertices(); i++ ) {
		graph->addVertex(QString::number(i));
	}


	preturn true;
}
