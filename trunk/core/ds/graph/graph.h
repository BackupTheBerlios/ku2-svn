/*
		graph.h
		Mon Apr 16 14:32:11 2007

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2007
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

/*!
	\file
	\brief Graph.
	
	This is the realisation of graphs.
	\author J. Anton
	\date Mon Apr 16 14:32:16 2007
	\version 1.0.0
*/

#ifndef KU__GRAPH_H__
#define KU__GRAPH_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/types.h"

//! Graph vertex.
typedef
struct STRUCT_GRAPH_VERTEX
{
	uint id;		//!< Vertex number/id.
	void *data;		//!< Vertex assotiated data.
	int tmp;		//!< Temporally data for usage by external algorithms.
}	graph_vertex_t;

#ifdef __cplusplus
}
#endif
#endif
