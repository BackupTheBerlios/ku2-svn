/*
 *	core:ds:graph:adjmatrix.h
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2011
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

#ifndef KU__DS_GRAPH_ADJMATRIX_H__
#define KU__DS_GRAPH_ADJMATRIX_H__
#include "ku2/host.h"

// Internal includes:
#include "ds/graph/graph.h"
#include "ds/matrix/int_matrix.h"

KU_BEGIN_DECLS

typedef
struct KU_STRUCT_GRAPH_ADJMATRIX
{
	ku_int_matrix_t *matrix;
	ku_flag32_t flags;
} ku_graph_am_t;

ku_graph_am_t *ku_graph_am_create( uint vertices, ku_flag32_t flags ) __THROW;
void ku_graph_am_free( ku_graph_am_t *graph ) __THROW;
void ku_graph_am_clear( ku_graph_am_t *graph ) __THROW;

kucode_t ku_graph_am_link( ku_graph_am_t *graph, uint from, uint to, int weight ) __THROW;
kucode_t ku_graph_am_ulink( ku_graph_am_t *graph, uint from, uint to ) __THROW;

int ku_graph_am_get_link( ku_graph_am_t *graph, uint from, uint to ) __THROW;

//! Insert some vertices.
kucode_t ku_graph_am_ins( ku_graph_am_t *graph, uint vertices ) __THROW;

//! Remove a vertex from the graph.
kucode_t ku_graph_am_rem( ku_graph_am_t *graph, uint vertex ) __THROW;

void ku_graph_am_printout( ku_graph_am_t *graph ) __THROW;

KU_END_DECLS
#endif // KU_DS_GRAPH_ADJMATRIX_H__
