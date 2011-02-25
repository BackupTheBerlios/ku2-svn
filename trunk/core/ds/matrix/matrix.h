/*
 *	core:ds:matrix:matrix.h
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2011
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

#ifndef KU__DS_MATRIX_H__
#define KU__DS_MATRIX_H__
#include "ku2/host.h"

// Internal includes:
#include "ku2/types.h"

KU_BEGIN_DECLS

/*
 * This define must exist and contain a real type name. This type will be
 * used for matrix data type.
 */
//#ifndef KU_MATRIX_TYPE
//#	error No matrix type defined!
//#endif
#define KU_MATRIX_TYPE int
/*
 * Function name creator in a form:
 *    ku_TYPE_matrix_FUNCTION
 * For instance, function for creating an integer matrix will be:
 *    ku_int_matrix_create
 */
#define __KU_MATRIX_F(__type, __func) \
	ku_ ## __type ## _matrix_ ## __func
#define _KU_MATRIX_F(__type, __func) __KU_MATRIX_F(__type, __func)
#define KU_MATRIX_F(__func) _KU_MATRIX_F(KU_MATRIX_TYPE, __func)

#define __KU_MATRIX_T(__type) \
		ku_ ## __type ## _matrix_t
#define _KU_MATRIX_T(__type) __KU_MATRIX_T(__type)
#define KU_MATRIX_T _KU_MATRIX_T(KU_MATRIX_TYPE)

#define _KU_STRUCT_MATRIX(__type) \
	KU_STRUCT_ ## __type ## _MATRIX
#define KU_STRUCT_MATRIX(__type) _KU_STRUCT_MATRIX(__type)

typedef struct KU_STRUCT_MATRIX(KU_MATRIX_TYPE)
{
	uint rows;             //!< Count of rows in the matrix.
	uint columns;          //!< Count of columns in the matrix.
	KU_MATRIX_TYPE data[0];//!< Pointer to the matrix data.
} KU_MATRIX_T;

KU_MATRIX_T *KU_MATRIX_F(create)( uint rows, uint columns,
                                  KU_MATRIX_TYPE filler ) __THROW;
void KU_MATRIX_F(free)( KU_MATRIX_T *matrix ) __THROW;
void KU_MATRIX_F(clear)( KU_MATRIX_T *matrix, int filler ) __THROW;

static inline KU_MATRIX_TYPE *KU_MATRIX_F(data)( KU_MATRIX_T *matrix, uint row, uint column ) {
	return matrix->data + row * matrix->columns + column;
}

KU_END_DECLS
#endif // KU__DS_MATRIX_H__
