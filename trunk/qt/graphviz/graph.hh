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

KU_BEGIN_DECLS

namespace graphviz {

class Graph
{
private:
	Context m_context;
};

}
KU_END_DECLS
#endif // KU_QT__GRAPHVIZ_GRAPH_HH__
