/*
 *	qt:graphviz:context.hh
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2010
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

//! \file

#ifndef KU_QT__GRAPHVIZ_CONTEXT_HH__
#define KU_QT__GRAPHVIZ_CONTEXT_HH__
#include <ku2/host.h>
//KU_ENSURE_CPLUSPLUS

#include <gvc.h>

KU_BEGIN_DECLS
namespace graphviz {

//! GraphViz context representation.
/*!
 * A singleton class for GV context representation.
 *
 * \internal
 * \since 2.0.0
 */
class Context
{
public:
	//! Constructor.
	Context();

public:
	//! Create an instance of the GV context.
	void instantiate();

	//! Cast operator to original GV context.
	inline operator GVC_t *() { return m_gvc; }

	//! GV context's selection through pointer operator.
	inline GVC_t *operator->() { return m_gvc; }

private:
	//! GV context.
	GVC_t* m_gvc;
};

}
KU_END_DECLS
#endif // KU_QT__GRAPHVIZ_CONTEXT_HH__
