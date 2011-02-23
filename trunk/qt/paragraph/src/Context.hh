/*
 *	qt:paragraph:Context.hh
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2011
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

#ifndef KU_QT__PARAGRAPH_CONTEXT_HH__
#define KU_QT__PARAGRAPH_CONTEXT_HH__
#include <ku2/host.h>
KU_ENSURE_CPLUSPLUS

// External includes:
#include <QObject>

namespace ku2 {
namespace paragraph {

// Class forward declarations:
class ContextHelper;
namespace graph {
	class Graph;
	class GraphModel;
}

class Context: public QObject
{
	Q_OBJECT
	friend class ContextHelper;
	Context();
	virtual ~Context();

public:
	static Context *instance();
	void showLastError() const;

public:
	bool isGraphOpen() const;
	bool newGraph();

public:
	void setChanged( bool changed = true );

signals:
	//! Notification that "changed" status has changed.
	void notifyChanged( bool newChanged );

public:
	graph::GraphModel *graphModel() const { return m_model; }

private:
	graph::Graph *m_graph;
	graph::GraphModel *m_model;
	bool m_hasChanges;
};

}}

#endif // KU_QT__PARAGRAPH_CONTEXT_HH__
