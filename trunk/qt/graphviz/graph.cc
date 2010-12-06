// Self-include:
#include "graph.hh"

// Ku2 includes:
#include "ku2/debug.h"

using namespace ku2::graphviz;

Graph::Graph(const QString &name):
		m_name(name)
{
	m_context.instantiate();

	// Default graph attributes:
	setMeta("overlap", "prism");
	setMeta("splines", "true");
	setMeta("pad", "0,2");
	setMeta("dpi", "96,0");
	setMeta("nodesep", "0,4");

	// Default attributes for the future nodes:
	nodeAttr("fixedsize", "true");
	nodeAttr("label", "");
	nodeAttr("regular", "true");

//	//Divide the wanted width by the DPI to get the value in points:
//	QString nodePtsWidth("%1").arg(node_size/_agget(_graph, "dpi", "96,0").toDouble());
//	//GV uses , instead of . for the separator in floats
//	_agnodeattr(_graph, "width", nodePtsWidth.replace('.', ","));

}

void Graph::initialise()
{
	open(m_name, GRAPH_DIRECTED);
}

/******************************************************************************
 * Manipulating nodes.                                                        *
 ******************************************************************************/

#define _qPrintable(__t) const_cast<char*>(qPrintable(__t))

void Graph::addNode(const QString &name)
{
	if ( m_nodes.contains(name) )
		removeNode(name);
	Agnode_t *node = agnode(m_graph, _qPrintable(name));
	ku_avoid_thr(node == NULL);
}


/******************************************************************************
 * Internal libgraph wrappers.                                                *
 ******************************************************************************/

void Graph::open(const QString &name, Types type)
{
	// Close the previously opened graph:
	if ( m_graph != NULL )
		close();

	// Set up flags:
	int flags;
	if ( type.testFlag(GRAPH_STRICT) ) {
		flags = type.testFlag(GRAPH_DIRECTED) ? AGDIGRAPHSTRICT
		                                      : AGRAPHSTRICT;
	} else {
		flags = type.testFlag(GRAPH_DIRECTED) ? AGDIGRAPH : AGRAPH;
	}

	// Open a graph:
	m_graph = agopen(const_cast<char*>(qPrintable(name)), flags);
	Q_ASSERT(m_graph != NULL);
}

void Graph::close()
{
	agclose(m_graph);
	m_graph = NULL;
}

QString Graph::meta(const QString &key)
{
	return agget(m_graph, const_cast<char*>(qPrintable(key)));
}

QString Graph::meta(const QString &key, const QString &defaultValue)
{
	QString mta = meta(key);
	return mta.isEmpty() ? defaultValue : mta;
}

int Graph::setMeta(const QString &key, const QString &value)
{
	return agsafeset(m_graph, const_cast<char*>(qPrintable(key)),
	                 const_cast<char*>(qPrintable(value)),
	                 const_cast<char*>(qPrintable(value)));
}

Agsym_t *Graph::nodeAttr(const QString &key, const QString &value)
{
	return agnodeattr(m_graph, const_cast<char*>(qPrintable(key)),
	                  const_cast<char*>(qPrintable(value)));
}
