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

// Ku2 includes:
#include "ku2/debug.h"

// External includes:
#include <QtCore>

using namespace ku2::graphviz;

class ContextHelper
{
public:
	ContextHelper();
	~ContextHelper();
public:
	inline GVC_t *data() const { return m_gvc; }

private:
	GVC_t *m_gvc;
};
ContextHelper::ContextHelper()
{
	m_gvc = gvContext();
}
ContextHelper::~ContextHelper()
{
	gvFreeContext(m_gvc);
}
Q_GLOBAL_STATIC(ContextHelper, context);

Context::Context():
		m_gvc(0)
{
	pdebug("GV context created");
}

void Context::instantiate()
{
	m_gvc = context()->data();
	pdebug("GV context instantiated as %p", m_gvc);
}
