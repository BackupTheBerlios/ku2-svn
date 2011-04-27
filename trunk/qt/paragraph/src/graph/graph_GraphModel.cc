/*
 *	qt:paragraph:graph:GraphModel.cc
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
#include "graph_GraphModel.hh"

// Internal includes:
#include "graph_Graph.hh"

// External includes:
#include <QModelIndex>
#include <QSize>

using namespace ku2::paragraph::graph;

GraphModel::GraphModel( Graph *graph, QObject *parent ):
		QAbstractTableModel(parent),
		m_graph(graph)
{
}

int GraphModel::rowCount( KU_UNUSED(const QModelIndex &parent) ) const
{
	return m_graph->vertices();
}

int GraphModel::columnCount( KU_UNUSED(const QModelIndex &parent) ) const
{
	return m_graph->vertices();
}

QVariant GraphModel::data( const QModelIndex &index, int role ) const
{
	if ( (index.row() < 0) || (index.row() >= rowCount()) )
		return QVariant();
	switch ( role ) {
	case Qt::SizeHintRole:
		return QSize(30, 30);
	case Qt::TextAlignmentRole:
		return Qt::AlignCenter;
	case Qt::DisplayRole:
		return m_graph->at(index.row(), index.column());
	}
	return QVariant();
}
