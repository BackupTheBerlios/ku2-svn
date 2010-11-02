/*
 *	adjacency_list.c
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2010
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

#include "adjacency_list.h"

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/gettext.h"

adjlist_t *ku_adjlist_create( uint nodes, uint start, uint end )
{
	adjlist_t *adjlist;
	uint local_nodes, i;
	pstartp("nodes: %u; start: %u; end: %u", nodes, start, end);
	
	// Checking ranges:
	if ( nodes == 0 )
		KU_ERRQ_V(MTE_RANGE, _("Cannot create a list without any node"), NULL);
	if ( start > end )
		KU_ERRQ_V(MTE_RANGE, _("Invalid start and end nodes"), NULL);
	if ( end > nodes )
		KU_ERRQ_V(MTE_RANGE, _("End node is beyond the range"), NULL);
	
	// Create adjlist:
	adjlist = dmalloc(sizeof(adjlist_t));
	if ( adjlist == NULL )
		KU_ERRQ_V(MTE_MEMORY, NULL, NULL);
	
	// Fill in data:
	adjlist->nodes_cnt = nodes;
	adjlist->start_node = start;
	adjlist->end_node = end;
	
	// Fill in isolated nodes:
	local_nodes = end - start + 1;
	adjlist->nodes = dmalloc(sizeof(adjlist_node_t) * local_nodes);
	if ( adjlist->nodes == NULL )
	{
		dfree(adjlist);
		KU_ERRQ_V(MTE_MEMORY, NULL, NULL);
	}
	for ( i = 0; i < local_nodes; i++ )
	{
		adjlist->nodes[i].edges_cnt = 0;
		adjlist->nodes[i].edges = NULL;
	}
	
	preturnp("%p", adjlist) adjlist;
}

void ku_adjlist_free( adjlist_t *adjlist )
{
	uint local_nodes, i;
	pstartp("adjlist: %p", adjlist);
	
	local_nodes = adjlist->end_node - adjlist->start_node + 1;
	for ( i = 0; i < local_nodes; i++ )
		if ( adjlist->nodes[i].edges )
			dfree(adjlist->nodes[i].edges);
	dfree(adjlist->nodes);
	dfree(adjlist);
	
	pstop();
}

// Check whether source and destination IDs are in range:
static inline int ku_adjlist_chk_range( adjlist_t *adjlist,
                                        uint src, uint dst )
{
	pstart();
	
	// Source node should be within the borders:
	if ( (src < adjlist->start_node) ||
	     (src > adjlist->end_node) )
		KU_ERRQ_V(MTE_RANGE, _("Source is out of range"), 0);
	
	// Destination should be within the borders:
	if ( dst >= adjlist->nodes_cnt )
		KU_ERRQ_V(MTE_RANGE, _("Destination is out of range"), 0);
	
	preturn 1;
}

mt_ecode_t adjlist_ins_edge( adjlist_t *adjlist,
                             graph_node_id_t src,
                             graph_node_id_t dst,
                             int weight,
                             graph_flag_t flags )
{
	adjlist_edge_t *edges;
	uint local_src;
	pstartp("adjlist: %p; src: %u; dst: %u; weight: %d; flags: %d",
	        adjlist, src, dst, weight, flags);
	
	// Are we in range:
	if ( !adjlist_chk_range(adjlist, src, dst) )
		preturnp("error") mt_gerrcode();
	
	// Getting space for a new edge:
	local_src = src - adjlist->start_node;
	edges = drealloc(adjlist->nodes[local_src].edges,
	                 sizeof(adjlist_edge_t) *
	                     (adjlist->nodes[local_src].edges_cnt + 1));
	if ( edges == NULL )
		MT_RETERR(MTE_MEMORY, _(MTET_MEMORY));
	adjlist->nodes[local_src].edges = edges;
	
	// Filling it:
	edges[adjlist->nodes[local_src].edges_cnt++].id = dst;
	edges[adjlist->nodes[local_src].edges_cnt].weight = weight;
	
	/*
		If we are asked to do the undirected edge,
		then connect from the other side.
	*/
	if ( MT_FLAG(flags, MT_GF_UNDIR) )
	{
		if ( adjlist_ins_edge(adjlist, dst, src, weight,
		                      flags&~MT_GF_UNDIR) != MTE_NONE )
		{
			// In case of error, memory is NOT deallocated:
			adjlist->nodes[local_src].edges_cnt--;
			preturnp("error") mt_gerrcode();
		}
	}
	
	preturnp("ok") MTE_NONE;
}

mt_ecode_t adjlist_rem_edge( adjlist_t *adjlist,
                             graph_node_id_t src,
                             graph_node_id_t dst,
                             int weight,
                             graph_flag_t flags )
{
	uint local_src, i;
	adjlist_edge_t *edges;
	pstartp("adjlist: %p; src: %u; dst: %u; weight: %d; flags: %u",
	        adjlist, src, dst, weight, flags);
	
	// Check for flag compatibiity:
	if ( MT_FLAG(flags, MT_GF_ANY) &&
	     MT_FLAG(flags, MT_GF_UNDIR) )
		MT_RETERR(MTE_INCOMPATIBLE,
		          _("Flag MT_GF_UNDIR cannot be used with flag MT_GF_ANY or "
		            "flag MT_GF_ALL"));
	
	// Are we in range:
	if ( !adjlist_chk_range(adjlist, src, dst) )
		preturnp("error") mt_gerrcode();
	
	// Search for the corresponding node:
	local_src = src - adjlist->start_node;
	edges = adjlist->nodes[local_src].edges;
	
	__search_next_edge:
	if ( MT_FLAG(flags, MT_GF_ANY) )
	{
		// Searching for a first edge to destination:
		for ( i = adjlist->nodes[local_src].edges_cnt - 1; i > 0; i-- )
			if ( edges[i].id == dst )
				goto __edge_was_found;
	}	else
	{
		// Searching for a first edge with the specific weight:
		for ( i = adjlist->nodes[local_src].edges_cnt - 1; i > 0; i-- )
			if ( (edges[i].id == dst) &&
			     (edges[i].weight == weight) )
				goto __edge_was_found;
	}
	
	// If we found nothing:
	/*
		If our purpose was not to delete all
		the edges, then return the error.
	*/
	if ( !MT_FLAG(flags, MT_GF_ALL) )
	{
		MT_RETERR(MTE_NOT_FOUND, _("Requested edge was not found"));
	}	else
	/*
		Else, we made our job.
	*/
	{
		goto __function_end;
	}
	
	__edge_was_found:
	// Shifting data (edge destinations):
	for ( i++; i < adjlist->nodes[local_src].edges_cnt; i++ )
		edges[i-1] = edges[i];
	adjlist->nodes[local_src].edges_cnt--;
	
	/*
		If we should delete ALL nodes, then
		go to the start of that function and try to delete one
		edge more.
	*/
	if ( MT_FLAG(flags, MT_GF_ALL) )
		goto __search_next_edge;
	
	__function_end:
	/*
		If we are asked to delete the edge in BOTH directions,
		then do it.
	*/
	if ( MT_FLAG(flags, MT_GF_UNDIR) )
	{
		if ( adjlist_rem_edge(adjlist, dst, src, weight,
		                      flags&~MT_GF_UNDIR) != MTE_NONE )
		{
			// Undelete the edge:
			__undelete_edge:
			edges[adjlist->nodes[local_src].edges_cnt].id = dst;
			edges[adjlist->nodes[local_src].edges_cnt++].weight = weight;
			preturnp("error") mt_gerrcode();
		}
	}
	
	/*
		If we are asked to leave the memory, do not touth it.
	*/
	if ( MT_FLAG(flags, MT_GF_LEAVE) )
	{
		adjlist_edge_t *realloced_edges = \
			drealloc(edges, sizeof(adjlist_edge_t) * \
			                adjlist->nodes[local_src].edges_cnt);
		if ( realloced_edges == NULL )
		{
			mt_serror(MTE_MEMORY, _(MTET_MEMORY));
			goto __undelete_edge;
		}
	}
	
	preturnp("ok") MTE_NONE;
}

/***************************************************************
	adjlist_load() helpers
***************************************************************/

static inline void adjlist_load_serror_read( int rsz )
{
	if ( rsz == -1 )
		mt_serror(MTE_DISK_IO, vstr(_(MTET_FILEREAD), strerror(errno))); else
		mt_serror(MTE_FORMAT, _("Invalid adjacency list file format"));
}

/*
	Read and check the file signature (0x34, 0x53, 0x23, 0x00) and return
	the total number of nodes in graph.

	Zero is returned upon error.
*/
static inline uint32_t read_signature_and_nodes( int fd )
{
	char buf[8];
	int rsz;
	
	pstartp("fd: %d", fd);

	// Reading signature and number of nodes:
	rsz = read(fd, buf, 8);
	if ( rsz != 8 )
		preturnp("error") adjlist_load_serror_read(rsz), 0;
	
	if ( (buf[0] != 0x34) ||
	     (buf[1] != 0x53) ||
	     (buf[2] != 0x23) ||
	     (buf[3] != 0x00) )
	{
		KU_ERRQ_V(MTE_FORMAT,
		            _("Invalid adjacency list file signature"), 0);
	}

	// Saving number of nodes:
	preturnp("nodes: %u", *((uint32_t*)(buf+4))) \
		*((uint32_t*)(buf+4));
}

/*
	Read the offsets of the nodes we are interested in. Go to the start
	node position. And return offsets (related to the start_node).

	Upon error return NULL.
*/
static inline uint32_t *read_offsets( int fd, graph_node_id_t start_node,
    uint local_nodes )
{
	int rsz;
	uint32_t *offsets;
	
	pstartp("fd: %d; start_node: %u; local_nodes: %u",
	        fd, start_node, local_nodes);

	// Allocate memory for offsets:
	offsets = dmalloc(sizeof(uint32_t) * local_nodes);
	if ( offsets == NULL )
		KU_ERRQ_V(MTE_MEMORY, _(MTET_MEMORY), NULL);
	
	// Go to the start node offset:
	if ( lseek(fd, 4*start_node, SEEK_CUR) == (off_t)-1 )
		goto __file_seek_error;

	// Read offsets:
	rsz = read(fd, offsets, 4*local_nodes);
	if ( rsz != 4*local_nodes )
	{
		dfree(offsets);
		preturnp("error") adjlist_load_serror_read(rsz), NULL;
	}
	
	// Move to the start node description:
	if ( lseek(fd,
	           offsets[0] - 4*(start_node+local_nodes),
	           SEEK_CUR) == (off_t)-1 )
		goto __file_seek_error;

	// Making the offset values related to start node:
	for ( rsz = 0; rsz < local_nodes; rsz++ )
		offsets[rsz] -= offsets[0];
	preturnp("offsets: %p", offsets) offsets;

	__file_seek_error:
	dfree(offsets);
	KU_ERRQ_V(MTE_DISK_IO, vstr(_(MTET_FILESEEK), strerror(errno)), NULL);
}

static inline adjlist_node_t *read_nodes( int fd,
                                          const uint32_t *offsets,
                                          uint local_nodes )
{
	adjlist_node_t *nodes;
	uint node_i, current_offset = 0;
	uint32_t edges_cnt;
	int rsz;

	pstartp("fd: %d; offsets: %p; local_nodes: %u", fd, offsets, local_nodes);

	// Allocating memory for nodes:
	nodes = dmalloc(sizeof(adjlist_node_t) * local_nodes);
	if ( nodes == NULL )
		KU_ERRQ_V(MTE_MEMORY, _(MTET_MEMORY), NULL);

	// Reading nodes:
	for ( node_i = 0; node_i < local_nodes; node_i++ )
	{
		uint edges_sz;
		
		// Checking offset:
		if ( offsets[node_i] != current_offset )
		{
			mt_serror(MTE_FORMAT,
			          _("Invalid adjacency list file node offsets"));
			goto __error;
		}

		// Reading the number of edges:
		rsz = read(fd, &edges_cnt, 4);
		if ( rsz != 4 )
		{
			adjlist_load_serror_read(rsz);
			goto __error;
		}
		current_offset += 4;

		edges_sz = sizeof(adjlist_edge_t) * edges_cnt;
		nodes[node_i].edges_cnt = edges_cnt;
		nodes[node_i].edges = dmalloc(edges_sz);
		if ( nodes[node_i].edges == NULL )
		{
			mt_serror(MTE_MEMORY, _(MTET_MEMORY));
			goto __error;
		}
		
		// Reading edges:
		rsz = read(fd, nodes[node_i].edges, edges_sz);
		if ( rsz != edges_sz )
		{
			adjlist_load_serror_read(rsz);
			goto __error_2;
		}
		current_offset += edges_sz;
	}
	
	preturnp("nodes: %p", nodes) nodes;
	
	__error:
	node_i--;
	
	__error_2:
	for ( ; node_i >= 0; node_i-- )
		dfree(nodes[node_i].edges);
	dfree(nodes);
	
	preturnp("error") NULL;
}

static inline adjlist_t *read_adjlist(int fd,
                                      uint32_t *offsets, uint local_nodes)
{
	adjlist_t *adjlist;
	
	pstartp("fd: %d; offsets: %p; local_nodes: %u",
	        fd, offsets, local_nodes);

	// Allocating memory for a list:
	adjlist = dmalloc(sizeof(adjlist_t));
	if ( adjlist == NULL )
		KU_ERRQ_V(MTE_MEMORY, _(MTET_MEMORY), NULL);

	// Read nodes:
	adjlist->nodes = read_nodes(fd, offsets, local_nodes);
	if ( adjlist->nodes == NULL )
	{
		dfree(adjlist);
		preturnp("error") NULL;
	}

	/*
		adjlist->nodes_cnt, ->start_node and ->end_node
		are set in adjlist_load
	*/
	preturnp("adjlist: %p", adjlist) adjlist;
}

adjlist_t *adjlist_load( const char *filename,
                         graph_node_id_t start_node,
                         graph_node_id_t end_node, mt_ioflag_t flags )
{
	int fd;
	
	adjlist_t *adjlist;
	uint nodes_cnt;
	uint local_nodes;
	uint32_t *offsets = NULL;
	
	pstartp("filename: %s; start_node: %u; end_node: %u",
	        filename, start_node, end_node);
	
	// Check for flags:
	if ( flags != MT_IOF_NOFLAGS )
		KU_ERRQ_V(MTE_RANGE, _("No flags should be passed"), NULL);
	
	// Open *.adjl file:
	if ( filename == NULL ) // use stdin:
	{
		fd = STDIN_FILENO;
	}	else
	{
		fd = open(filename, O_RDONLY);
		if ( fd == -1 )
			KU_ERRQ_V(MTE_DISK_IO,
			            vstr(_("Failed to open adjacency list file: %s"),
			                 strerror(errno)), NULL)
	}
	
	// Check for file signature and get the number of nodes:
	nodes_cnt = read_signature_and_nodes(fd);
	if ( nodes_cnt == 0 )
		goto __error;
	
	// Setting up start and end nodes:
	if ( end_node > nodes_cnt )
		end_node = nodes_cnt;
	if ( start_node > end_node )
	{
		mt_serror(MTE_RANGE, _("Start node is greater than end node"));
		goto __error;
	}
	local_nodes = end_node-start_node+1;

	// Moving to the start of the node description:
	offsets = read_offsets(fd, start_node, local_nodes);
	if ( offsets == NULL )
		goto __error;
	
	// Creating adjacency list:
	adjlist = read_adjlist(fd, offsets, local_nodes);
	if ( adjlist == NULL )
		goto __error;
	adjlist->nodes_cnt = nodes_cnt;
	adjlist->start_node = start_node;
	adjlist->end_node = end_node;

	dfree(offsets);
	close(fd);
	preturnp("adjlist: %p", adjlist) adjlist;
	
	__error:
	if ( offsets )
		dfree(offsets);
	close(fd);
	preturnp("error") NULL;
}

mt_ecode_t adjlist_save( const char *filename, mt_ioflag_t flags )
{
}
