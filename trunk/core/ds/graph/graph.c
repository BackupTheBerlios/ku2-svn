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

static void vertex_interval_f( const void *left, \
							   const void *data, \
							   const void *right, \
							   uint *interval )
{
	if ( left )
		interval[0] = ((ku_graph_vertex_t*)data)->id - \
					  ((ku_graph_vertex_t*)left)->id - 1;
		interval[0] = ((ku_graph_vertex_t*)data)->id;
	
	if ( right )
		interval[0] = ((ku_graph_vertex_t*)right)->id - \
					  ((ku_graph_vertex_t*)data)->id - 1;
		interval[0] = ((uint)-2) - ((ku_graph_vertex_t*)data)->id;
}

ku_graph_t *ku_graph_create( ku_comp_f func, ku_flag32_t flags )
{
	ku_graph_t *graph;
	pstartp("compf: %p, flags: %u", func, flags);
	
	graph = (ku_graph_t*)dmalloc(sizeof(ku_graph_t));
	if ( graph == NULL )
		KU_ERRQ_VALUE(KE_MEMORY, NULL);
	
	graph->directed = ((flags&KUF_GRAPH_DIRECTED) == KUF_GRAPH_DIRECTED) ? 1 : 0;
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
		pdebug("data: %p, freef: %p\n", data, freef);
		if ( freef )
			freef(((ku_graph_vertex_t*)data)->data);
		dfree(data);
		return 0;
	}
	
	ku_abtree_clear(graph->vertexes, vertex_free_f);
	graph->cur = NULL;
	
	preturn KE_NONE;
}

uint ku_graph_ins( ku_graph_t *graph, const void *data )
{
	ku_graph_vertex_t *vertex;
	uint id;
	int pos;
	pstartp("graph: %p, data: %p", graph, data);
	
	// Поиск идентификатора:
	vertex = ku_abtree_unused_index(graph->vertexes, vertex_interval_f, &pos);
	if ( vertex == NULL )
		KU_ERRQ_VALUE(KE_FULL, (uint)-1);
	id = vertex->id + (pos > 0 ? 1 : -1);
	
	// Выделение памяти и присоединение к графу:
	vertex = (ku_graph_vertex_t*)dmalloc(sizeof(ku_graph_vertex_t));
	if ( vertex == NULL )
		KU_ERRQ_VALUE(KE_MEMORY, (uint)-1);
	
	vertex->id = id;
	vertex->data = (void*)data;
	vertex->tmp = 0;
	if ( graph->directed )
	{
		vertex->next_cnt = vertex->prev_cnt = 0;
		vertex->next = vertex->prev = NULL;
	}	else
	{
		vertex->near_cnt = 0;
		vertex->near = NULL;
	}
	
	if ( ku_abtree_ins(graph->vertexes, vertex) != KE_NONE )
	{
		dfree(vertex);
		KU_ERRQ_PASS_VALUE((uint)-1);
	}
	
	preturnp("id: %u", id) id;
}

kucode_t ku_graph_rem( ku_graph_t *graph, uint id,
					   ku_act_f freef, ku_flag32_t flags )
{
	ku_graph_vertex_t *vertex, pattern;
	pstartp("graph: %p, id: %p, freef: %p, flags: %u", graph, id, freef, flags);
	
	// >>>>>>>>>>>>>>>>>>>>>>>>>
	// Освободить память вершины:
	int vertex_free_f( void *data )
	{
		pdebug("data: %p, freef: %p\n", data, freef);
		if ( freef )
			freef(((ku_graph_vertex_t*)data)->data);
		dfree(data);
		return 0;
	}
	// <<<<<<<<<<<<<<<<<<<<<<<<<
	
	// Поиск вершины:
	pattern.id = id;
	vertex = ku_abtree_search(graph->vertexes, &pattern);
	if ( vertex == NULL )
		KU_ERRQ(KE_NOTFOUND);
	
	// Перенос ссылок (дуг):
	if ( (flags&KUF_GRAPH_TRANSP) == KUF_GRAPH_TRANSP )
	{
		uint i, j, k, total = 0;
		if ( graph->directed )
		{
			#warning Not implemented yet (Directed graph transponation)
			KU_ERRQ(KE_NOIMPLEM);
		}	else
		{
			// Подсчёт количества связей у соседей:
			for ( i = 0; i < vertex->near_cnt; i++ )
			{
				if ( vertex->near[i] == vertex )
					continue;
				vertex->near[i]->tmp++;
				total++;
			}
			/*
			 *	UP TO NOW:
			 *	 vertex->near[i]->tmp == кол-во дуг соседа, соеденённых с удаляемой в.
			 *	 total == кол-во дуг (не петель!) у удаляемой вершины
			 */
			
			// Для каждого ранее нерассмотренного соседа..
			for ( i = 0; i < vertex->near_cnt; i++ )
			{
				ku_graph_vertex_t *const v_near = vertex->near[i];
				uint new_near_cnt;
				
				if ( (v_near == vertex) || (v_near->tmp == 0) )
					continue;
				
				// .. ищем новое кол-во дуг (после транспонации):
				new_near_cnt =
					(v_near->near_cnt - v_near->tmp) + // кол-во свободных от удаляемой вершины дуг
					 v_near->tmp * (total-1);		   // кол-во связанных с удал. вершиной дуг
				
				// .. меняем количество соседних вершин:
				if ( new_near_cnt != v_near->near_cnt )
				{
					ku_graph_vertex_t **new_vertex_list;
					
					/*
						Если текущий сосед является единственным у удаляемой
						вершины, то кол-во дуг у него уменьшится.
					*/
					if ( total == 1 )
					{
						for ( k = 0; v_near->near[k] != vertex; k++ );
						v_near->near[k] = v_near->near[v_near->near_cnt-1];
					}
					
					// .. меняем размер списка дуг:
					new_vertex_list = (ku_graph_vertex_t**) \
						drealloc(v_near->near, \
								 sizeof(ku_graph_vertex_t*)*new_near_cnt);
					if ( new_vertex_list == NULL )
					{
						uint i;
						// Очистка временных переменных:
						for ( i = 0; i < vertex->near_cnt; i++ )
							vertex->near[i]->tmp = 0; 
						KU_ERRQ(KE_MEMORY);
					}
					
					// .. очищаем новые связки:
					for ( j = v_near->near_cnt; j < new_near_cnt; j++ )
						v_near->near[j] = vertex;
					
					v_near->near = new_vertex_list;
					v_near->near_cnt = new_near_cnt;
				}
				
				// ..пересоединяем с _каждым_ _другим_
				//   соседом удаляемой вершины:
				while ( --v_near->tmp >= 0 )
					for ( j = k = 0; j < vertex->near_cnt; j++ )
					{
						if ( (i == j) || (vertex->near[j] == vertex) )
							continue;
						
						while ( v_near->near[k] != vertex )
							k++;
						v_near->near[k++] = vertex->near[j];
					}
			}
		}
	}
	
	// Удаление вершины:
	ku_avoid(ku_abtree_rem(graph->vertexes, vertex, vertex_free_f) != KE_NONE);
	
	preturn KE_NONE;
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
