/***************************************************************************
 *            memory.h
 *
 *  Mon Aug 28 12:15:35 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Memory allocation functions and definitions.
	
	This file contains the functions and definitions for memory allocations.
	\author J. Anton
	\date Mon Aug 28 12:16:34 2006
	\version 1.1.0
*/

#ifndef KU__MEMORY_H__
#define KU__MEMORY_H__
#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG
#include <stdlib.h>

void *ku_malloc_debug( size_t size );
void ku_free_debug( void *__ptr );
void ku_memory_stat( void );

#define dmalloc( size ) \
ku_malloc_debug(size)

#define dfree( ptr ) \
ku_free_debug(ptr)

#define dlogmemstat() \
ku_memory_stat()

#else	//	DEBUG
#include <stdlib.h>

//! Allocate memory.
/*!
	Allocates \a size bytes of memory.
	\param size Size to allocate.
	\note If \b DEBUG is defined then the function will count the amount of
	allocations and the total memory has been allocated. If \b DEBUG_MEMORY
	is defined then debug information will be written to the \e stdout.
	\sa dfree() and dlogmemstat().
*/
#define dmalloc( size ) malloc(size)

//! Free memory.
/*!
	Frees a memory pointed by \a ptr.
	\param ptr Pointer to the memory being freed.
	\note If \b DEBUG is defined then the function will count the amount of
	allocations and the total memory has been allocated. If \b DEBUG_MEMORY
	is defined then debug information will be written to the \e stdout.
	\sa dmalloc() and dlogmemstat().
*/
#define dfree( ptr ) free(ptr)

//! Logs a memory status.
/*!
	Logs currently allocation statistics (\b DEBUG mode): bytes and blocks allocated.
	\note openlog() should be called before this function.
	\sa dmalloc() and dfree().
*/
#define dlogmemstat()

#endif	//	DEBUG

#ifdef __cplusplus
}
#endif
#endif
