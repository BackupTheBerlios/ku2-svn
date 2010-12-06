/*
 *	qt:graphviz:graph.hh
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2010
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

/*!
 * \file
 * \author J. Anton
 * \brief Host computer system specific parameters.
 *
 * Host computer system specific parameters. OS definitions, compiler
 * definitions, etc.
 */

#ifndef KU_QT__GRAPHVIZ_GRAPH_HH__
#define KU_QT__GRAPHVIZ_GRAPH_HH__
#include <ku2/host.h>
KU_ENSURE_CPLUSPLUS

// Internal includes:
#include "context.hh"

// External includes:
#include <QtGlobal>
#include <QStringList>
#include <QMap>

KU_BEGIN_DECLS
namespace graphviz {

class Q_DECL_EXPORT Graph
{
public:
	//! Default DPI value used by "dot".
	static const qreal defaultDpi;

	//! Default node size of the graph.
	static const qreal defaultNodeSize;

	enum Type {
		GRAPH_DIRECTED = 0x0001,
		GRAPH_STRICT   = 0x0002
	};
	Q_DECLARE_FLAGS(Types, Type);

public:
	//! Construct a graphviz graph object.
	/*!
	 * Constructs an empty graphviz graph object with default parameters.
	 *
	 * \param name Name of the graph.
	 */
	Graph(const QString &name);

	//! Destroy a graph object.
	~Graph();

	//! Initialise the graph.
	void initialise();

	//! Clear a graph.
	void clear();

public:
	//! Add a graph node.
	void addNode(const QString &name);

	//! Add many graph nodes.
	void addNodes(const QStringList &names);

	//! Remove a graph node.
	void removeNode(const QString &name);

	//! Add an edge.
	void addEdge(const QString &source, const QString &dest);

	//! Remove an edge.
	void removeEdge(const QString &source, const QString &dest);

private:
	//! Wrapper for agopen(char*,int).
	void open(const QString &name, Types type);

	void close();

	QString meta(const QString &key);
	QString meta(const QString &key, const QString &defaultValue);

	//! Wrapper for agset(void*,char*,char*).
	int setMeta(const QString &key, const QString &value);

	//!  and agnodeattr(void*,char*,char*)
	Agsym_t *nodeAttr(const QString &key, const QString &value);

private:
	//! Graph context.
	Context m_context;

	//! Graph name.
	QString m_name;

	//! Graph representation.
	Agraph_t *m_graph;

	//! List of the nodes.
	QMap<QString,Agnode_t*> m_nodes;
};

}
KU_END_DECLS

Q_DECLARE_OPERATORS_FOR_FLAGS(ku2::graphviz::Graph::Types)

#endif // KU_QT__GRAPHVIZ_GRAPH_HH__
