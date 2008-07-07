/*
		graph.c
		Mon Apr 16 14:32:01 2007

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2007
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/memory.h"
#include "ds/graph/graph.h"

static int vertex_comp_f( const void *a, const void *b )
{
	return ((ku_graph_vertex_t*)a)->id - ((ku_graph_vertex_t*)b)->id;
}

ku_graph_t *ku_graph_create( ku_comp_f func, ku_flag32_t flags )
{
	ku_graph_t *graph;
	pstartp("compf: %p, flags: %u", func, flags);
	
	graph = (ku_graph_t*)dmalloc(sizeof(ku_graph_t));
	if ( graph == NULL )
		KU_ERRQ_VALUE(KE_MEMORY, NULL);
	
	graph->orient = ((flags&KUF_GRAPH_ORIENT) == KUF_GRAPH_ORIENT) ? 1 : 0;
	graph->cmpf = func;
	graph->cur = NULL;
	if ( (graph->vertexes = ku_abtree_create(vertex_comp_f, 0)) == NULL )
	{
		dfree(graph);
		KU_ERRQ_PASS_VALUE(NULL);
	}
	
	preturnp("graph: %p", graph) graph;
}

kucode_t ku_graph_free( ku_graph_t *graph, ku_act_f freef )
{
	pstartp("graph: %p, freef: %p", graph, freef);
	
	ku_graph_clear(graph, freef);
	ku_abtree_free(graph->vertexes, NULL);
	dfree(graph);
	
	preturn KE_NONE;
}

kucode_t ku_graph_clear( ku_graph_t *graph, ku_act_f freef )
{
	pstartp("graph: %p, freef: %p", graph, freef);
	
	int vertex_free_f( void *data )
	{
		_
		return 0;
	}
	
	ku_abtree_clear(graph->vertexes, vertex_free_f);
	graph->cur = NULL;
	
	preturn KE_NONE;
}

uint ku_graph_ins( ku_graph_t *graph, const void *data )
{
	pstartp("graph: %p, data: %p", graph, data);
	KU_ERRQ_VALUE(KE_NOIMPLEM, (uint)-1);
}

kucode_t ku_graph_rem( ku_graph_t *graph, uint id,
					   ku_act_f freef, ku_flag32_t flags )
{
	pstartp("graph: %p, id: %p, freef: %p, flags: %u", graph, id, freef, flags);
	KU_ERRQ(KE_NOIMPLEM);
}

kucode_t ku_graph_link( ku_graph_t *graph,
					    uint start_node, uint end_node, ku_flag32_t flags )
{
	pstartp("graph: %p, start: %u, end: %u, flags: %u",
			graph, start_node, end_node, flags);
	KU_ERRQ(KE_NOIMPLEM);
}

kucode_t ku_graph_push( ku_graph_t *graph )
{
	pstartp("graph: %p", graph);
	KU_ERRQ(KE_NOIMPLEM);
}

kucode_t ku_graph_pop( ku_graph_t *graph )
{
	pstartp("graph: %p", graph);
	KU_ERRQ(KE_NOIMPLEM);
}

void *ku_graph_push_goto_next( ku_graph_t *graph, uint index )
{
	pstartp("graph: %p, index: %u", graph, index);
	KU_ERRQ_VALUE(KE_NOIMPLEM, NULL);
}

void *ku_graph_goto_next( ku_graph_t *graph, uint index )
{
	pstartp("graph: %p, index: %u", graph, index);
	KU_ERRQ_VALUE(KE_NOIMPLEM, NULL);
}

void *ku_graph_push_goto_prev( ku_graph_t *graph, uint index )
{
	pstartp("graph: %p, index: %u", graph, index);
	KU_ERRQ_VALUE(KE_NOIMPLEM, NULL);
}

void *ku_graph_goto_prev( ku_graph_t *graph, uint index )
{
	pstartp("graph: %p, index: %u", graph, index);
	KU_ERRQ_VALUE(KE_NOIMPLEM, NULL);
}

void *ku_graph_search( ku_graph_t *graph, uint id )
{
	pstartp("graph: %p, id: %u", graph, id);
	KU_ERRQ_VALUE(KE_NOIMPLEM, NULL);
}

void *ku_graph_search_by_data( ku_graph_t *graph, const void *data )
{
	pstartp("graph: %p, data: %p", graph, data);
	KU_ERRQ_VALUE(KE_NOIMPLEM, NULL);
}
