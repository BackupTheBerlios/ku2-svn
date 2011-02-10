/*
 *	qt:graphviz:node.hh
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2010
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

#ifndef KU_QT__GRAPHVIZ_NODE_HH__
#define KU_QT__GRAPHVIZ_NODE_HH__
#include <ku2/host.h>
KU_ENSURE_CPLUSPLUS

// External includes:
#include <QtGlobal>
#include <QPoint>
#include <QString>
#include <QSharedDataPointer>
#include <QSharedData>

KU_BEGIN_DECLS
namespace graphviz {
class Q_DECL_EXPORT Node;

class NodePrivate: public QSharedData
{
	friend class ku2::graphviz::Node;

private:
	NodePrivate( const QString &name,
	             const QPoint &centre,
	             qreal width, qreal height );

public:
	inline QString name() const { return m_name; }
	inline QPoint centre() const { return m_centre; }
	inline qreal width() const { return m_width; }
	inline qreal height() const { return m_height; }

private:
	QString m_name;
	QPoint m_centre;
	qreal m_width;
	qreal m_height;
};

class Node
{
public:
	Node( const QString &name, const QPoint &centre,
	      qreal width, qreal height );

public:
	inline QString name() const { return d_ptr->name(); }
	inline QPoint centre() const { return d_ptr->centre(); }
	inline qreal width() const { return d_ptr->width(); }
	inline qreal height() const { return d_ptr->height(); }

private:
	QSharedDataPointer<NodePrivate> d_ptr;
};

}
KU_END_DECLS

#endif // KU_QT__GRAPHVIZ_NODE_HH__
