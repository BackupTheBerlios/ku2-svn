/*
 *	memory.h
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2006
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

/*!
 * \file
 * \author J. Anton
 * \date Mon Aug 28 12:16:34 2006
 * \brief Memory allocation functions and definitions.
 *
 * This file contains the functions and definitions for memory allocations.
 */

#ifndef KU__MEMORY_H__
#define KU__MEMORY_H__
#include "host.h"
KU_BEGIN_DECLS

#include <stdlib.h>

#ifdef KU_DEBUG
// Debug malloc:
void *ku_malloc_debug( size_t size );
#define dmalloc( size ) \
ku_malloc_debug(size)

// Debug free:
void ku_free_debug( void *__ptr );
#define dfree( ptr ) \
ku_free_debug(ptr)

// Debug realloc:
void *ku_realloc_debug( void *__ptr, size_t size );
#define drealloc( ptr, size ) \
ku_realloc_debug(ptr, size)

// Print memory status:
void ku_memory_stat( void );
#define dlogmemstat() \
ku_memory_stat()

#else	// DEBUG
//! Allocate memory.
/*!
	Allocates \a size bytes of memory.
	\param size Size to allocate.
	\note If \b DEBUG is defined then the function will count the amount of
	allocations and the total memory has been allocated. If \b DEBUG_MEMORY
	is defined then debug information will be written to the \e stdout.
	\sa malloc(), drealloc(), dfree() and dlogmemstat().
*/
#define dmalloc( size ) malloc(size)

//! Free memory.
/*!
	Frees a memory pointed by \a ptr.
	\param ptr Pointer to the memory being freed.
	\note If \b DEBUG is defined then the function will count the amount of
	allocations and the total memory has been allocated. If \b DEBUG_MEMORY
	is defined then debug information will be written to the \e stdout.
	\sa free(), dmalloc() drealloc() and dlogmemstat().
*/
#define dfree( ptr ) free(ptr)

//! Reallocate the memory.
/*!
 * Reallocates the memory pointed by \a ptr to a new \a size.
 * \param ptr Pointer to the memory being reallocated.
 * \param size New size of the memory.
 * \note If \b DEBUG is defined then the function will count the amount of
 *       allocations and the total memory has been allocated. If \b DEBUG_MEMORY
 *       is defined then debug information will be written to the \e stdout.
 * \sa realloc(), dmalloc(), dfree() and dlogmemstat().
 */
#define drealloc( ptr, size ) realloc(ptr, size)

//! Logs a memory status.
/*!
	Logs currently allocation statistics (\b DEBUG mode): bytes and blocks allocated.
	\note openlog() should be called before this function.
	\sa dmalloc() and dfree().
*/
#define dlogmemstat()

#endif	// DEBUG

KU_END_DECLS
#endif
