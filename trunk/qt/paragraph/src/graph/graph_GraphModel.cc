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

// External includes:
#include <QModelIndex>

using namespace ku2::paragraph::graph;

GraphModel::GraphModel( QObject *parent ):
		QAbstractTableModel(parent)
{
}

int GraphModel::rowCount( const QModelIndex &parent ) const
{
	return 10;
}

int GraphModel::columnCount( const QModelIndex &parent ) const
{
	return 10;
}

QVariant GraphModel::data( const QModelIndex &index, int role ) const
{
	if ( (index.row() < 0) || (index.row() >= rowCount()) )
		return QVariant();
	return 1;
}
