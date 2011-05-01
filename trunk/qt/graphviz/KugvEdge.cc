/*
 *	qt:graphviz:edge.cc
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
#include "KugvEdge.hh"

using namespace ku2::graphviz;

EdgePrivate::EdgePrivate( const QString &fromName, const QString &toName,
                          const QPainterPath &path ):
	m_fromName(fromName), m_toName(toName), m_path(path)
{
}

Edge::Edge( const QString &fromName, const QString &toName,
            const QPainterPath &path ):
	d_ptr(new EdgePrivate(fromName, toName, path))
{
}
