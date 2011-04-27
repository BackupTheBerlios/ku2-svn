/*
 *	qt:paragraph:Context.cc
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
#include "Context.hh"

// Internal includes:
#include "graph/graph_Graph.hh"
#include "graph/graph_GraphModel.hh"

// Ku2 includes:
#include <ku2/ecode.h>

// External includes:
#include <QtGlobal>
#include <QBasicAtomicPointer>
#include <QMessageBox>

// Singleton helper:
namespace ku2 {
namespace paragraph {
class ContextHelper
{
public:
	Context m_context;
};
}}
Q_GLOBAL_STATIC(ku2::paragraph::ContextHelper, contextHelper)

using namespace ku2;
using namespace ku2::paragraph;
using namespace ku2::paragraph::graph;



Context::Context():
		m_graph(NULL),
		m_model(NULL),
		m_hasChanges(false)
{
}

Context::~Context()
{
	if ( m_graph )
		delete m_graph;
	if ( m_model )
		delete m_model;
}

Context *Context::instance()
{
	return &contextHelper()->m_context;
}

void Context::showLastError() const
{
	const char *const text = ku_gerrtx();
	const char *const name = ku_gerrname();

	QMessageBox::critical(NULL,
	    //% "Error"
	    //: This is the title of the error message box.
	    qtTrId("qtn_error_message_title"),

	    //% "<b>An internal error %1 has occurred.</b><p>%2"
	    qtTrId("qtn_error_message").arg(name).arg(text));
}

bool Context::isGraphOpen() const
{
	return !m_graph;
}

bool Context::newGraph()
{
	pstart();

	// Do not allow to create the second graph:
	if ( m_graph != NULL )
		KU_ERRQNT_V(KE_DOUBLE, false);

	m_graph = new Graph();
	m_model = new GraphModel(m_graph);

	emit notifyOpened();
	setChanged(true);
	preturn true;
}

bool Context::visualiseGraph()
{
	pstart();

	if ( m_graph == NULL )
		KU_ERRQNT_V(KE_EMPTY, false);

	preturn true;
}

void Context::setChanged( bool changed )
{
	if ( changed != m_hasChanges ) {
		m_hasChanges = changed;
		emit notifyChanged(m_hasChanges);
	}
}
