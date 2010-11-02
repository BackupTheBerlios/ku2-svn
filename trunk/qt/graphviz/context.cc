/*
 *	qt:graphviz:context.cc
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
#include "context.hh"

// External includes:
#include <QtCore>

using namespace ku2::graphviz;
Q_GLOBAL_STATIC(ContextHelper, context);

class ContextHelper
{
public:
	inline ContextHelper() {
		m_gvc = gvContext();
	}
	inline ~ContextHelper() {
		gvFreeContext(m_gvc);
	}
	inline GVC_t *data() const { return m_gvc; }

private:
	GVC_t *m_gvc;
};

void Context::instantiate()
{
	m_gvc = context()->data();
}


