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
					//!< Note: algorith should set zeros after its work.
	union
	{
		struct
		{
			uint next_cnt,
					//!< Count of next graph vertexes.
				prev_cnt;
					//!< Count of previous graph vertexes.
			struct STRUCT_GRAPH_VERTEX
				**next,
					//!< Next graph vertexes.
				**prev;
					//!< Previous graph vertexes.
		};
		struct
		{
			uint near_cnt;
					//!< Count of linked (near) graph vertexes.
			struct STRUCT_GRAPH_VERTEX
				**near;
					//!< Linked (near) graph vertexes.
		};
	};
}	ku_graph_vertex_t;

//! Graph.
typedef
struct STRUCT_GRAPH
{
	int directed;	//!< Is this graph oriented?
	ku_comp_f cmpf;	//!< Comparing function.
	ku_tree_t *vertexes;
					//!< List of vertexes, indexed by id.
	ku_graph_vertex_t *cur;
					//!< Current vertex.
}	ku_graph_t;

//! \name Graph flags.
//! \{
/*!
	Graph should be directed. \n
	Used in ku_graph_create().
*/
#define KUF_GRAPH_DIRECTED	1

/*!
	- Create links in both directions when linking.
	Has effect only if graph is directed. \n
	Used in ku_graph_link().
	
	- Remove links in both directions when unlinking.
	Has effect only if graph is directed. \n
	Used in ku_graph_ulink().
*/
#define KUF_GRAPH_DBL_LINK	2

/*!
	When removing the vertex from the graph, transpose the links, where
	it is possible, for example if:
	- three vertexes are connected as folowing: A -> B -> C.
	- B is being deleted.
	
	then there will be link A -> C is after the operation if this flag is used.
	If this flag is not used then the links will be simply deleted. \n
	Used in ku_graph_rem().
*/
#define KUF_GRAPH_TRANSP	4

/*!
	Remove all links between two vertexes rather then only one. \n
	Used in ku_graph_ulink().
*/
#define KUF_GRAPH_ALL_LINK	8
//! \}

//! Create a graph instance.
ku_graph_t *ku_graph_create( ku_comp_f func, ku_flag32_t flags );

//! Free a graph instance.
kucode_t ku_graph_free( ku_graph_t *graph, ku_act_f freef );

//! Remove all vertexes from the graph.
kucode_t ku_graph_clear( ku_graph_t *graph, ku_act_f freef );

//! Insert the data to the graph.
/*!
	Creates a vertex with data and inserts it into the graph. New vertex
	is not linked to any other.
	\param graph Graph to deal with.
	\param data Data to be inserted.
	\return ID of the created vertex, or (uint)-1 if failed and \ref kucode is
	set to the valid value: \n
	- \e KE_FULL There is no free ID. \n
	- \e KE_MEMORY Failed to allocate space for vertex. \n
	- Any ku_abtree_ins() error.
*/
uint ku_graph_ins( ku_graph_t *graph, const void *data );

//! Remove a particular vertex (using its ID) from the graph.
/*!
	Removes a vertex with specified ID from the graph.
	\param graph Graph to deal with.
	\param id Vertex ID to be deleted.
	\param freef Function to free vertex data, \e NULL for doing nothing.
	\param flags Flags. \n Valid flags are: \ref KUF_GRAPH_TRANSP.
	\retval KE_NONE No error.
	\retval KE_NOTFOUND No vertex with specified ID was found.
	\retval KE_MEMORY Failed to reallocate space for list of linked vertexes.
*/
kucode_t ku_graph_rem( ku_graph_t *graph, uint id,
					   ku_act_f freef, ku_flag32_t flags );

//! Link two vertexes together (using their IDs).
/*!
	Links two vertexes together (using their IDs).
	\param graph Graph to deal with.
	\param start_node ID of the linking start node.
	\param end_node ID of the end node:
		the node to which start node is linked.
	\param flags Flags. \n Valid flags are: \ref KUF_GRAPH_DBL_LINK.
	\retval KE_NONE No error.
	\retval KE_NOTFOUND No vertex with specified ID was found.
	\retval KE_MEMORY Failed to reallocate space for list of linked vertexes.
*/
kucode_t ku_graph_link( ku_graph_t *graph,
					    uint start_node, uint end_node, ku_flag32_t flags );

//! Unlink two linked vertexes (using their IDs).
kucode_t ku_graph_ulink( ku_graph_t *graph,
						 uint start_node, uint end_node, ku_flag32_t flags );

kucode_t ku_graph_push( ku_graph_t *graph );
kucode_t ku_graph_pop( ku_graph_t *graph );

void *ku_graph_push_goto_next( ku_graph_t *graph, uint index );
void *ku_graph_goto_next( ku_graph_t *graph, uint index );
void *ku_graph_push_goto_prev( ku_graph_t *graph, uint index );
void *ku_graph_goto_prev( ku_graph_t *graph, uint index );

void *ku_graph_search( ku_graph_t *graph, uint id );
void *ku_graph_search_by_data( ku_graph_t *graph, const void *data );

ku_graph_vertex_t *ku_graph_get_vertex( ku_graph_t *graph, uint id );

void ku_graph_printout( ku_graph_t *graph, ku_actP_f outf );

KU_END_DECLS
#endif
