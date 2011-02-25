/*
 *	core:ds:graph:adjmatrix.c
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2011
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

// Self-include:
#include "adjmatrix.h"

// Internal includes:
#include "ku2/debug.h"
#include "ku2/memory.h"
#include "ku2/gettext.h"

// External includes:
#include <stdio.h>

ku_graph_am_t *ku_graph_am_create( uint vertices, ku_flag32_t flags )
{
	pstartp("vertices = %u, flags = %u", vertices, flags);
	ku_graph_am_t *graph = dmalloc(sizeof(ku_graph_am_t));
	if ( graph == NULL )
		KU_ERRQNT_V(KE_MEMORY, NULL);
	graph->flags = flags;
	graph->matrix = ku_int_matrix_create(vertices, vertices, 0);
	if ( graph->matrix == NULL ) {
		dfree(graph);
		preturn NULL;
	}
	preturnp("graph = %p", graph) graph;
}

void ku_graph_am_free( ku_graph_am_t *graph )
{
	ku_int_matrix_free(graph->matrix);
	dfree(graph);
}

void ku_graph_am_clear( ku_graph_am_t *graph )
{
	ku_int_matrix_clear(graph->matrix, 0);
}

kucode_t ku_graph_am_link( ku_graph_am_t *graph, uint from, uint to, int weight )
{
	pstartp("graph = %p, from = %u, to = %u, weight = %d",
	        graph, from, to, weight);
	uint vertices = graph->matrix->rows;
	if ( (from >= vertices) || (to >= vertices) )
		KU_ERRQ(KE_RANGE, _("The requested vertex is out of graph"));
	*ku_int_matrix_data(graph->matrix, from, to) = weight;
	preturn KE_NONE;
}

kucode_t ku_graph_am_ulink( ku_graph_am_t *graph, uint from, uint to )
{
	pstart();
	preturn ku_graph_am_link(graph, from, to, 0);
}

int ku_graph_am_get_link( ku_graph_am_t *graph, uint from, uint to )
{
	pstartp("graph = %p, from = %u, to = %u", graph, from, to);
	uint vertices = graph->matrix->rows;
	if ( (from >= vertices) || (to >= vertices) )
		KU_ERRQ(KE_RANGE, _("The requested vertex is out of graph"));
	int weight = *ku_int_matrix_data(graph->matrix, from, to);
	preturnp("weight = %d", weight) weight;
}

kucode_t ku_graph_am_ins( ku_graph_am_t *graph, uint vertices )
{
	KU_DO_NOT_USE(graph);
	KU_DO_NOT_USE(vertices);
	return ku_serror(KE_NOIMPLEM, NULL);
}

kucode_t ku_graph_am_rem( ku_graph_am_t *graph, uint vertex )
{
	KU_DO_NOT_USE(graph);
	KU_DO_NOT_USE(vertex);
	return ku_serror(KE_NOIMPLEM, NULL);
}

void ku_graph_am_printout( ku_graph_am_t *graph )
{
	int *weight = ku_int_matrix_data(graph->matrix, 0, 0);
	uint row, column;

	// Heading:
	printf("\n     |");
	for ( column = 0; column < graph->matrix->columns; column++ )
		printf(" %3u |", column + 1);
	printf("\n");
	for ( column = 0; column < graph->matrix->columns; column++ )
		printf("-----+");
	printf("-----|\n");

	// Contents:
	for ( row = 0; row < graph->matrix->rows; row++ ) {
		printf("%4u |", row + 1);
		for ( column = 0; column < graph->matrix->columns; column++ ) {
			printf(" %3u |", *weight);
			weight++;
		}
		printf("\n");
	}

	// Footer:
	for ( column = 0; column < graph->matrix->columns; column++ )
		printf("-----^");
	printf("-----/\n\n");
}
