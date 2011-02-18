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

void Graph::setRootNode( const QString &name )
{
	setMeta("root", name);
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
	gvLayout(m_context, m_graph, "twopi");//"dot");
}

QRectF Graph::boundingRect() const
{
	qreal dpi = toRealHelper(meta("dpi", "96,0"));
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
	const qreal dpi = metaDpi();
	const qreal adjustedDpi = adjustDpi(dpi);

	foreach ( Agnode_t *agNode, m_nodes ) {
		qreal x = agNode->u.coord.x * adjustedDpi;
		qreal y = (m_graph->u.bb.UR.y - agNode->u.coord.y) * adjustedDpi;

		printf("NODE %s [%f,%f] [%f,%f]\n", agNode->name, x, y,
		       agNode->u.coord.x, agNode->u.coord.y);

		Node node(QString(agNode->name), QPoint(x, y),
		          agNode->u.width * dpi, agNode->u.height * dpi);
		list.append(node);
	}

	pstop();
	return list;
}

QList<Edge> Graph::edges() const
{
	pstart();
	QList<Edge> list;
	const qreal adjustedDpi = adjustDpi(metaDpi());
	const double height = m_graph->u.bb.UR.y;

	foreach ( Agedge_t *agEdge, m_edges ) {
		QPainterPath path;

		bezier *bz = agEdge->u.spl->list;
		if ( bz && (bz->size % 3) ) {
			if ( bz->sflag ) {
				path.moveTo(bz->sp.x * adjustedDpi,
				            (height - bz->sp.y) * adjustedDpi);
				path.lineTo(bz->list[0].x * adjustedDpi,
				            (height - bz->list[0].y) * adjustedDpi);
			} else {
				path.moveTo(bz->list[0].x * adjustedDpi,
				            (height - bz->list[0].y) * adjustedDpi);
			}

			for ( int i = 1; i < bz->size; i += 3 ) {
				path.cubicTo(bz->list[i].x * adjustedDpi,
				             (height - bz->list[i].y) * adjustedDpi,
				             bz->list[i+1].x * adjustedDpi,
				             (height - bz->list[i+1].y) * adjustedDpi,
				             bz->list[i+2].x * adjustedDpi,
				             (height - bz->list[i+2].y) * adjustedDpi);
			}

			if ( bz->eflag ) {
				path.lineTo(bz->ep.x * adjustedDpi,
				            (height - bz->ep.y) * adjustedDpi);
			}
		}

		list << Edge(agEdge->tail->name, agEdge->head->name, path);
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
#ifdef KU2_DEBUG
	QString ret = agget(m_graph, _qPrintable(key));
	printf("META %s = %s\n", qPrintable(key), qPrintable(ret));
	return ret;
#else
	return agget(m_graph, const_cast<char*> (qPrintable(key)));
#endif
}

QString Graph::meta( const QString &key, const QString &defaultValue ) const
{
	QString mta = meta(key);
	return mta.isEmpty() ? defaultValue : mta;
}

qreal Graph::toRealHelper( QString value ) const
{
	bool ok;
	qreal ret = value.replace(',', '.').toDouble(&ok);
	ku_avoid_thr(!ok)
	return ret;
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
