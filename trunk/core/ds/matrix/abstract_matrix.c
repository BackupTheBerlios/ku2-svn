/*
 *	core:ds:matrix:abstract_matrix.c
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
#include "abstract_matrix.h"

// Internal includes:
#include "ku2/debug.h"
#include "ku2/ecode.h"
#include "ku2/memory.h"

KU_MATRIX_T *KU_MATRIX_F(create)( uint rows, uint columns, KU_MATRIX_TYPE filler )
{
	pstartp("rows = %u, columns = %u", rows, columns);
	const uint dim = rows * columns;
	KU_MATRIX_T *matrix =
		dmalloc(sizeof(KU_MATRIX_T) + sizeof(KU_MATRIX_TYPE) * dim);
	if ( matrix == NULL )
		KU_ERRQNT_V(KE_MEMORY, NULL);

	matrix->rows = rows;
	matrix->columns = columns;
	KU_MATRIX_F(clear)(matrix, filler);
	preturnp("matrix = %p", matrix) matrix;
}

void KU_MATRIX_F(free)( KU_MATRIX_T *matrix )
{
	pstartp("matrix = %p", matrix);
	dfree(matrix);
	pstop();
}

void KU_MATRIX_F(clear)( KU_MATRIX_T *matrix, KU_MATRIX_TYPE filler )
{
	uint dim = matrix->rows * matrix->columns;
	KU_MATRIX_TYPE *data = matrix->data;
	for ( /* none */; dim > 0; dim-- ) {
		data[dim - 1] = filler;
	}
}
