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
	\brief Linked graph.
	
	This is the realisation of linked graphs.
	\author J. Anton
*/

#ifndef KU__GRAPH_H__
#define KU__GRAPH_H__
#include "ku2/host.h"
KU_BEGIN_DECLS

#include "ku2/types.h"
#include "ds/abtree/abtree.h"

//! Graph vertex.
typedef
struct STRUCT_GRAPH_VERTEX
{
	uint id;		//!< Vertex number/id.
	void *data;		//!< With vertex assotiated data.
	int tmp;		//!< Temporally data for usage by external algorithms.
	union
	{
		struct
		{
			uint next_cnt,
					//!< Count of next graph vertexes.
				prev_cnt;
					//!< Count of previous graph vertexes.
			struct STRUCT_GRAPH_VERTEX
				*next,
					//!< Next graph vertexes.
				*prev;
					//!< Previous graph vertexes.
		};
		struct
		{
			uint near_cnt;
					//!< Count of linked (near) graph vertexes.
			struct STRUCT_GRAPH_VERTEX
				*near;
					//!< Linked (near) graph vertexes.
		};
	};
}	ku_graph_vertex_t;

//! Graph.
typedef
struct STRUCT_GRAPH
{
	int orient;		//!< Is this graph oriented?
	ku_comp_f cmpf;	//!< Comparing function.
	ku_tree_t *vertexes;
					//!< List of vertexes, indexed by id.
	ku_graph_vertex_t *cur;
					//!< Current vertex.
}	ku_graph_t;

//! Graph flags.
//! {
#define KUF_GRAPH_ORIENT	1 // Oriented graph..
#define KUF_GRAPH_DBL_LINK	2 // When linking, create links in both directions..
/*
	When removing, transpose the links, if possible:
	A -> B -> C; B is deleted ==> A -> C is after the operation
*/
#define KUF_GRAPH_TRANSP	4
//! }

ku_graph_t *ku_graph_create( ku_comp_f func, ku_flag32_t flags );
kucode_t ku_graph_free( ku_graph_t *graph, ku_act_f freef );
kucode_t ku_graph_clear( ku_graph_t *graph, ku_act_f freef );

uint ku_graph_ins( ku_graph_t *graph, const void *data );
kucode_t ku_graph_rem( ku_graph_t *graph, uint id,
					   ku_act_f freef, ku_flag32_t flags );

kucode_t ku_graph_link( ku_graph_t *graph,
					    uint start_node, uint end_node, ku_flag32_t flags );

kucode_t ku_graph_push( ku_graph_t *graph );
kucode_t ku_graph_pop( ku_graph_t *graph );

void *ku_graph_push_goto_next( ku_graph_t *graph, uint index );
void *ku_graph_goto_next( ku_graph_t *graph, uint index );
void *ku_graph_push_goto_prev( ku_graph_t *graph, uint index );
void *ku_graph_goto_prev( ku_graph_t *graph, uint index );

void *ku_graph_search( ku_graph_t *graph, uint id );
void *ku_graph_search_by_data( ku_graph_t *graph, const void *data );

KU_END_DECLS
#endif
