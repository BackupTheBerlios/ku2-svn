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
