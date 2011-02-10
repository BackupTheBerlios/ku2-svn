/*
 *	qt:graphviz:node.cc
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
#include "node.hh"

using namespace ku2::graphviz;

NodePrivate::NodePrivate( const QString &name, const QPoint &centre,
                          qreal width, qreal height ):
		m_name(name), m_centre(centre), m_width(width), m_height(height)
{
}

Node::Node( const QString &name, const QPoint &centre,
            qreal width, qreal height ):
		d_ptr(new NodePrivate(name, centre, width, height))
{
}
