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

#include <stdio.h>

#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/memory.h"
#include "ds/graph/graph.h"

static const ku_graph_vertex_t second_min_vertex = {
	.id = 1
};

static int vertex_comp_f( const void *a, const void *b )
{
	return ((ku_graph_vertex_t*)a)->id - ((ku_graph_vertex_t*)b)->id;
}

static void *vertex_interval_f( const void *left, \
							    const void *data, \
							    const void *right, \
							    uint *interval )
{
	if ( data == NULL )
		return (void*)(&second_min_vertex);
	
	if ( left )
		interval[0] = ((ku_graph_vertex_t*)data)->id - \
					  ((ku_graph_vertex_t*)left)->id - 1;	else
		interval[0] = ((ku_graph_vertex_t*)data)->id;
	
	if ( right )
		interval[1] = ((ku_graph_vertex_t*)right)->id - \
					  ((ku_graph_vertex_t*)data)->id - 1;	else
		interval[1] = ((uint)-2) - ((ku_graph_vertex_t*)data)->id;
	
	return NULL;
}

ku_graph_t *ku_graph_create( ku_comp_f func, ku_flag32_t flags )
{
	ku_graph_t *graph;
	pstartp("compf: %p, flags: %u", func, flags);

	graph = (ku_graph_t*)dmalloc(sizeof(ku_graph_t));
	if ( graph == NULL )
		KU_ERRQNT_V(KE_MEMORY, NULL);

	graph->directed
	                = ((flags & KUF_GRAPH_DIRECTED) == KUF_GRAPH_DIRECTED)
	                                                                       ? 1
	                                                                       : 0;
	graph->cmpf = func;
	graph->cur = NULL;
	if ( (graph->vertexes = ku_abtree_create(vertex_comp_f, 0)) == NULL ) {
		dfree(graph);
		return NULL;
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
		KU_ERRQNT_V(KE_FULL, (uint)-1);
	id = vertex->id + (pos > 0 ? 1 : -1);
	
	// Выделение памяти и присоединение к графу:
	vertex = (ku_graph_vertex_t*)dmalloc(sizeof(ku_graph_vertex_t));
	if ( vertex == NULL )
		KU_ERRQNT_V(KE_MEMORY, (uint)-1);
	
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
		return (uint)-1;
	}
	
	preturnp("id: %u", id) id;
}

kucode_t ku_graph_rem( ku_graph_t *graph, uint id,
					   ku_act_f freef, ku_flag32_t flags )
{
	ku_graph_vertex_t *vertex;
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
	vertex = ku_graph_get_vertex(graph, id);
	if ( vertex == NULL )
		KU_ERRQNT(KE_NOTFOUND);
	
	// Перенос ссылок (дуг):
	if ( (flags&KUF_GRAPH_TRANSP) == KUF_GRAPH_TRANSP )
	{
		uint i, j, k, total = 0;
		if ( graph->directed )
		{
			#warning Not implemented yet (Directed graph transponation)
			KU_ERRQNT(KE_NOIMPLEM);
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
						KU_ERRQNT(KE_MEMORY);
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
	}	else
	{
		#warning Links should be removed here!
	}
	
	// Удаление вершины:
	ku_avoid(ku_abtree_rem(graph->vertexes, vertex, vertex_free_f) != KE_NONE);
	
	preturn KE_NONE;
}

/*
	Соединить 2 вершины.
	В случае ошибки памяти функция возвращает -1, при этом,
		изначальная структура вершин не меняется.
	В случае успеха, функция возвращает 0.
*/
static inline int __ku_graph_link( ku_graph_vertex_t *vertex,
								   ku_graph_vertex_t *dest,
								   int directed )
{
	ku_graph_vertex_t
		***next, **next_vl,		// List of Next Vertexes
		***prev, **prev_vl;		// List of Previous Vertexes
	uint *next_cnt, *prev_cnt;
	
	if ( directed )
	{
		next = &vertex->next;
		next_cnt = &vertex->next_cnt;
		prev = &dest->prev;
		prev_cnt = &dest->prev_cnt;
	}	else
	{
		next = &vertex->near;
		next_cnt = &vertex->near_cnt;
		prev = &dest->near;
		prev_cnt = &dest->near_cnt;
	}
	
	next_vl = (ku_graph_vertex_t**) \
		drealloc(*next, sizeof(ku_graph_vertex_t*) * (vertex->next_cnt+1));
	if ( next_vl == NULL )
		return -1;
	
	prev_vl = (ku_graph_vertex_t**) \
		drealloc(*prev, sizeof(ku_graph_vertex_t*) * (dest->prev_cnt+1));
	if ( prev_vl == NULL )
	{
		// Возвращаем vertex->next на место:
		*next = (ku_graph_vertex_t**) \
			drealloc(next_vl, sizeof(ku_graph_vertex_t*) * vertex->next_cnt);
		if ( *next == NULL )
		{
			// Если вдруг не удалось обратно уменьшить размер выделенной
			// памяти под vertex->next, то оставляем в пямяти место под
			// один лишний элемент, который не будет учитываться другими
			// функциями (vertex->next_cnt остаётся неизменным)
			*next = next_vl;
		}
		return -1;
	}
	
	(*next = next_vl)[(*next_cnt)++] = dest;
	(*prev = prev_vl)[(*prev_cnt)++] = vertex;
	
	return 0;
}

#warning If start_node and end_node are the same, then there is an error..
kucode_t ku_graph_link( ku_graph_t *graph,
					    uint start_node, uint end_node, ku_flag32_t flags )
{
	ku_graph_vertex_t *start, *end;
	pstartp("graph: %p, start: %u, end: %u, flags: %u",
			graph, start_node, end_node, flags);
	
	if ( ((start = ku_graph_get_vertex(graph, start_node)) == NULL) ||
		 ((end = ku_graph_get_vertex(graph, end_node)) == NULL) )
		KU_ERRQNT(KE_NOTFOUND);
	
	// Создаём связку:
	if ( __ku_graph_link(start, end, graph->directed) == -1 )
		KU_ERRQNT(KE_MEMORY);
	
	// Если необходимо, создаём связку в обратную сторону:
	if ( graph->directed && ((flags&KUF_GRAPH_DBL_LINK) == KUF_GRAPH_DBL_LINK) )
	{
		if ( __ku_graph_link(end, start, 1) == -1 )
		{
#warning Here should be restored status quo
			KU_ERRQNT(KE_MEMORY);
		}
	}
	
	preturn KE_NONE;
}

kucode_t ku_graph_ulink( ku_graph_t *graph,
						 uint start_node, uint end_node, ku_flag32_t flags )
{
	pstartp("graph: %p, start: %u, end: %u, flags: %u",
			graph, start_node, end_node, flags);
	KU_ERRQNT(KE_NOIMPLEM);
}

kucode_t ku_graph_push( ku_graph_t *graph )
{
	pstartp("graph: %p", graph);
	KU_ERRQNT(KE_NOIMPLEM);
}

kucode_t ku_graph_pop( ku_graph_t *graph )
{
	pstartp("graph: %p", graph);
	KU_ERRQNT(KE_NOIMPLEM);
}

void *ku_graph_push_goto_next( ku_graph_t *graph, uint index )
{
	pstartp("graph: %p, index: %u", graph, index);
	KU_ERRQNT_V(KE_NOIMPLEM, NULL);
}

void *ku_graph_goto_next( ku_graph_t *graph, uint index )
{
	pstartp("graph: %p, index: %u", graph, index);
	KU_ERRQNT_V(KE_NOIMPLEM, NULL);
}

void *ku_graph_push_goto_prev( ku_graph_t *graph, uint index )
{
	pstartp("graph: %p, index: %u", graph, index);
	KU_ERRQNT_V(KE_NOIMPLEM, NULL);
}

void *ku_graph_goto_prev( ku_graph_t *graph, uint index )
{
	pstartp("graph: %p, index: %u", graph, index);
	KU_ERRQNT_V(KE_NOIMPLEM, NULL);
}

void *ku_graph_search( ku_graph_t *graph, uint id )
{
	pstartp("graph: %p, id: %u", graph, id);
	KU_ERRQNT_V(KE_NOIMPLEM, NULL);
}

void *ku_graph_search_by_data( ku_graph_t *graph, const void *data )
{
	pstartp("graph: %p, data: %p", graph, data);
	KU_ERRQNT_V(KE_NOIMPLEM, NULL);
}

ku_graph_vertex_t *ku_graph_get_vertex( ku_graph_t *graph, uint id )
{
	ku_graph_vertex_t *vertex, pattern;
	pstartp("graph: %p, id: %u", graph, id);
	
	pattern.id = id;
	vertex = ku_abtree_search(graph->vertexes, &pattern);
	
	preturnp("vertex: %p", vertex) vertex;
}

void ku_graph_printout( ku_graph_t *graph, ku_actP_f outf )
{
	ku_graph_vertex_t *vertex;
	pstartp("graph: %p, outf: %p", graph, outf);
	
	ku_abtree_goto_first(graph->vertexes);
	
	while ( (vertex = ku_abtree_goto_next(graph->vertexes)) != NULL )
	{
		uint i;
		
		if ( outf )
			printf("ID: %u; DATA: %s; ", \
				   vertex->id, (char*)outf(vertex->data)); else
			printf("ID: %u; ", vertex->id);
		
		if ( graph->directed )
		{
			printf("NEXT: ");
			if ( vertex->next_cnt > 0 )
			{
				for ( i = 0; i < vertex->next_cnt; i++ )
					printf("%u; ", vertex->next[i]->id);
			}	else
				printf("(none); ");
			printf("PREV: ");
			if ( vertex->prev_cnt > 0 )
			{
				for ( i = 0; i < vertex->prev_cnt; i++ )
					printf("%u; ", vertex->prev[i]->id);
			}	else
				printf("(none); ");
			printf("\n");
		}	else
		{
			printf("LINK: ");
			if ( vertex->near_cnt > 0 )
			{
				for ( i = 0; i < vertex->near_cnt; i++ )
					printf("%u; ", vertex->near[i]->id);
				printf("\n");
			}	else
				printf("(none)\n");
		}
	}
	
	pstop();
}
