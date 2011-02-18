/*
 *	qt:graphviz:edge.hh
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2011
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

#ifndef KU_QT__GRAPHVIZ_EDGE_HH__
#define KU_QT__GRAPHVIZ_EDGE_HH__
#include <ku2/host.h>
KU_ENSURE_CPLUSPLUS

// External includes:
#include <QPainterPath>
#include <QSharedData>
#include <QSharedDataPointer>
#include <QString>

KU_BEGIN_DECLS
namespace graphviz {
class Q_DECL_EXPORT Edge;

class EdgePrivate: public QSharedData
{
	friend class ku2::graphviz::Edge;

private:
	EdgePrivate( const QString &fromName,
	             const QString &toName,
	             const QPainterPath &path );

public:
	inline const QString &fromName() const { return m_fromName; }
	inline const QString &toName() const { return m_toName; }
	inline const QPainterPath &path() const { return m_path; }

private:
	QString m_fromName;
	QString m_toName;
	QPainterPath m_path;
};

class Edge
{
public:
	Edge( const QString &fromName, const QString &toName,
	      const QPainterPath &path );

public:
	inline const QString &fromName() const { return d_ptr->fromName(); }
	inline const QString &toName() const { return d_ptr->toName(); }
	inline const QPainterPath &path() const { return d_ptr->path(); }

private:
	QSharedDataPointer<EdgePrivate> d_ptr;
};

}
KU_END_DECLS

#endif // KU_QT__GRAPHVIZ_EDGE_HH__
