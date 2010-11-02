/*
 *	adjacency_list.h
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2010
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

/*!
 * \file
 * \author J. Anton
 * \date September 2010
 * \brief Static adjacency list graph representation with weighed edges.
 *
 * \since 2.0.0
 */

#ifndef KU__ADJACENCY_LIST_H__
#define KU__ADJACENCY_LIST_H__
#include "host.h"
KU_BEGIN_DECLS

//! Outgoing edge of the node.
typedef
struct
{
	uint id;               //!< ID of destination node.
	int weight;            //!< Weight of the edge.
}	adjlist_edge_t;

//! Graph node representation in the adjacency list.
typedef
struct
{
	uint edges_cnt;        //!< Number of outgoing edges.
	adjlist_edge_t *edges; //!< List of outgoing edges.
}	adjlist_node_t;

//! Adjacency list.
typedef
struct
{
	uint nodes_cnt;     //!< Total number of nodes.
	graph_node_id_t start_node;
	                    //!< First node that is hold in list.
	graph_node_id_t end_node;
	                    //!< Last node that is hold in list.
	adjlist_node_t *nodes;
	                    //!< List of nodes.
}	adjlist_t;

//! Create and adjacency list.
/*!
 * Creates and adjacency list with the predefined number of nodes.
 * \param nodes Total number of nodes in graph.
 * \param start Start (first) node Id that is stored in adjacency list.
 * \param end End (last) node ID that is stored in adjacency list.
 * \return Adjacency list instance, or \e NULL in case of error:
 *         \n \a MTE_RANGE: \t Invalid \e start and \e end values.
 *         \n \a MTE_MEMORY: \t Out of memory.
 * \sa ku_adjlist_free(), ku_adjlist_load() and ku_adjlist_save().
 */
adjlist_t *ku_adjlist_create( uint nodes, uint start, uint end );

//! Free an adjacency list.
/*!
 * Frees an adjacency list instance.
 * \param adjlist adjacency list to be freed.
 * \sa ku_adjlist_create(), ku_adjlist_load() and ku_adjlist_save().
 */
void ku_adjlist_free( adjlist_t *adjlist );

//! Insert an edge to the adjacency list.
/*!
	Inserts an edge to the adjacency list.
	\param adjlist Adjacency list to work with.
	\param src Source node ID.
	\param dst Destination node ID.
	\param weight Edge weight.
	\param flags Graph flags:
	\n \a MT_GF_UNDIR: \t Make an edge in both directions.
	\retval MTE_NONE Edge was inserted.
	\retval MTE_RANGE Node IDs are not in possible range.
	\retval MTE_MEMORY Out of memory.
	\sa adjlist_rem_edge().
*/
kucode_t ku_adjlist_ins_edge( adjlist_t *adjlist,
                             graph_node_id_t src,
                             graph_node_id_t dst,
                             int weight,
                             graph_flag_t flags );

//! Remove an edge to the adjacency list.
/*!
	Removes an edge to the adjacency list.
	\param adjlist Adjacency list to work with.
	\param src Source node ID.
	\param dst Destination node ID.
	\param weight Edge weight that will be deleted.
	\param flags Graph flags:
	\n \a MT_GF_UNDIR: \t Remove an edge in both directions.
	\n \a MT_GF_LEAVE: \t Do not free the memory.
	\n \a MT_GF_ANY: \t Remove an edge from \e src to \e dst of any weight,
		parameter \e weight is ignoed. Cannot be used with \a MT_GF_UNDIR.
	\n \a MT_GF_ALL: \t Remove all edges from \e src to \e dst. Parameter
		\e weight is ignored. Cannot be used with \a MT_GF_UNDIR.
	\retval MTE_NONE Edge was deleted.
	\retval MTE_INCOMPATIBLE Invalid flag combination.
	\retval MTE_RANGE Node IDS are not in possible range.
	\retval MTE_NOT_FOUND Required edge (or edges) was not found.
	\retval MTE_MEMORY Memory reallocation failed.
	\sa adjlist_ins_edge().
*/
mt_ecode_t adjlist_rem_edge( adjlist_t *adjlist,
                             graph_node_id_t src,
                             graph_node_id_t dst,
                             int weight, 
                             graph_flag_t flags );

//! Load an adjacency list from the file.
/*!
	Load an adjacency list from the file.
	\param filename Path to the file. To use \e stdin, special value of
		\e NULL should be passed.
	\param start_node First node that the list will contain.
	\param end_node Last node that the list will contain. To read the whole
		graph, special value of -1 should be passed.
	\param flag IO flags.
	\return Adjacency list instance, or NULL in case of errors:
	\n \a MTE_OPEN: \t Failed to open a specified file.
	\n \a MTE_FORMAT: Invalid file structure.
	\n \a MTE_MEMORY: Memory error.
	\n \a MTE_RANGE: Start node is greater that total number of nodes.
		Or start node is greater than end node.
	\n \a MTE_READ: File read error.
	\note Parameter \e flag is ignored.
	\note If end node is greater than the total number of nodes then file is
		read until the end.
	\sa adjlist_save(), adjlist_create() and adjlist_free().
*/
adjlist_t *adjlist_load( const char *filename,
                         graph_node_id_t start_node,
                         graph_node_id_t end_node, mt_ioflag_t flags );

//! Save an adjacency list to the file.
/*!
	Saves an adjacency list to the file.
	\param filename Path to the file.
	\retval MTE_NONE Save succeeded.
	\retval MTE_*.
	\sa adjlist_load(), adjlist_create() and adjlist_free().
*/
mt_ecode_t adjlist_save( const char *filename, mt_ioflag_t flags );

__MT_END_DECLS
#endif

