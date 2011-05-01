/*
 *	qt:paragraph:graph:Graph.hh
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2011
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

#ifndef KU_QT__PARAGRAPH_GRAPH_GRAPH_HH__
#define KU_QT__PARAGRAPH_GRAPH_GRAPH_HH__
#include <ku2/host.h>
KU_ENSURE_CPLUSPLUS

// Ku2 includes:
#include <ds/graph/adjmatrix.h>

namespace ku2 {
namespace paragraph {
namespace graph {

class Graph
{
public:
	Graph( uint vertices = 10 );
	~Graph();

public:
	inline uint cntVertices() const {
		return m_graph->matrix->rows;
	}

	inline int at( uint from, uint to ) const {
		return ku_graph_am_get_link(m_graph, from, to);
	}

	//! Visualise the graph on the provided scene.
	bool visualise( QGraphicsScene *scene ) const;

private:
	ku_graph_am_t *m_graph;
};

}}}
#endif // KU_QT__PARAGRAPH_GRAPH_GRAPH_HH__
