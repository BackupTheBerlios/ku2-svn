/*
 *	qt:paragraph:graph:GraphModel.hh
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2011
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

#ifndef KU_QT__PARAGRAPH_GRAPH_GRAPH_MODEL_HH__
#define KU_QT__PARAGRAPH_GRAPH_GRAPH_MODEL_HH__
#include <ku2/host.h>
KU_ENSURE_CPLUSPLUS

// External includes:
#include <QAbstractTableModel>

KU_BEGIN_DECLS
namespace paragraph {
namespace graph {
class Graph;

class GraphModel: public QAbstractTableModel
{
public:
	GraphModel( Graph *graph, QObject *parent = NULL );

public:
	int rowCount( const QModelIndex &parent = QModelIndex() ) const;
	int columnCount( const QModelIndex &parent = QModelIndex() ) const;
	QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;

private:
	Graph *m_graph;
};

}}
KU_END_DECLS

#endif // KU_QT__PARAGRAPH_GRAPH_GRAPH_MODEL_HH__
