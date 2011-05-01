/*
 *	qt:graphviz:vertex.cc
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2010
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

// Self-include:
#include "KugvVertex.hh"

using namespace ku2::graphviz;

VertexPrivate::VertexPrivate( const QString &name, const QPoint &centre,
                          qreal width, qreal height ):
		m_name(name), m_centre(centre), m_width(width), m_height(height)
{
}

Vertex::Vertex( const QString &name, const QPoint &centre,
            qreal width, qreal height ):
		d_ptr(new VertexPrivate(name, centre, width, height))
{
}
