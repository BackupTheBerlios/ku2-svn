// Self-include:
#include "graph.hh"

// Ku2 includes:
#include "ku2/debug.h"

using namespace ku2::graphviz;

const qreal Graph::dotDefaultDpi = 72.0;

Graph::Graph( const QString &name ) :
	m_name(name),
	m_graph(NULL)
{
	m_context.instantiate();
}

Graph::~Graph()
{

}

void Graph::initialise()
{
	open(m_name, GRAPH_DIRECTED);

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

/******************************************************************************
 * Manipulating nodes and edges.                                              *
 ******************************************************************************/

#define _qPrintable(__t) const_cast<char*>(qPrintable(__t))

void Graph::addNode( const QString &name )
{
//	if ( m_nodes.contains(name) )
//		removeNode(name);
	Agnode_t *node = agnode(m_graph, _qPrintable(name));
	ku_avoid_thr(node == NULL);
	m_nodes.insert(name, node);
}

void Graph::addNodes( const QStringList &names )
{
	foreach ( const QString &name, names ) {
		addNode(name);
	}
}

void Graph::addEdge( const QString &source, const QString &dest )
{
	if ( m_nodes.contains(source) && m_nodes.contains(dest) ) {
		QPair<QString, QString> edge(source, dest);
		if ( !m_edges.contains(edge) ) {
			m_edges.insert(edge, agedge(m_graph,
			                            m_nodes.value(source),
			                            m_nodes.value(dest)));
		}
	}
}

/******************************************************************************
 * Layouts.                                                                   *
 ******************************************************************************/

void Graph::applyLayout( Layout layout )
{
	gvFreeLayout(m_context, m_graph);
	switch ( layout ) {
	case LAYOUT_DOT:
		applyDotLayout();
		break;
	}
}

void Graph::applyDotLayout()
{
	gvLayout(m_context, m_graph, "dot");
}

QRectF Graph::boundingRect() const
{
	qreal dpi = meta("dpi", "96,0").toDouble();
	return QRectF(m_graph->u.bb.LL.x * (dpi / dotDefaultDpi),
	              m_graph->u.bb.LL.y * (dpi / dotDefaultDpi),
	              m_graph->u.bb.UR.x * (dpi / dotDefaultDpi),
	              m_graph->u.bb.UR.y * (dpi / dotDefaultDpi));
}


/******************************************************************************
 * Getting nodes and edges.                                                   *
 ******************************************************************************/

QList<Node> Graph::nodes() const
{
	pstart();
	QList<Node> list;
	qreal dpi = meta("dpi", "96,0").toDouble();

	foreach ( Agnode_t *agNode, m_nodes ) {
		qreal x = agNode->u.coord.x * (dpi / dotDefaultDpi);
		qreal y = (m_graph->u.bb.UR.y - agNode->u.coord.y) *
		          (dpi / dotDefaultDpi);

		Node node(QString(agNode->name), QPoint(x, y),
		          agNode->u.width * dpi, agNode->u.height * dpi);
		list.append(node);
	}

	pstop();
	return list;
}


/******************************************************************************
 * Internal libgraph wrappers.                                                *
 ******************************************************************************/

void Graph::open( const QString &name, Types type )
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
	m_graph = agopen(_qPrintable(name), flags);
	Q_ASSERT(m_graph != NULL);
}

void Graph::close()
{
	agclose(m_graph);
	m_graph = NULL;
}

QString Graph::meta( const QString &key ) const
{
	return agget(m_graph, const_cast<char*> (qPrintable(key)));
}

QString Graph::meta( const QString &key, const QString &defaultValue ) const
{
	QString mta = meta(key);
	return mta.isEmpty() ? defaultValue : mta;
}

int Graph::setMeta( const QString &key, const QString &value )
{
	return agsafeset(m_graph, _qPrintable(key),
	                 _qPrintable(value), _qPrintable(value));
}

Agsym_t *Graph::nodeAttr( const QString &key, const QString &value )
{
	return agnodeattr(m_graph, const_cast<char*> (qPrintable(key)),
	                  const_cast<char*> (qPrintable(value)));
}
