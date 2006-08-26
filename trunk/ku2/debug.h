/***************************************************************************
 *            debug.h
 *
 *  Thu Aug 17 23:15:55 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Debug functions and definitions.
	
	This file contains the functions and definitions for debugging.
	\author J. Anton
	\date Thu Aug 17 23:17:34 2006
	\version 1.1.0
*/

#ifndef KU__DEBUG_H__
#define KU__DEBUG_H__
#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG

void printf_debug( const char *file, const char *func, int line, char *fmt, ... );
void *malloc_debug( int size );
void free_debug( void *__ptr );

#define pdebug( m, ... ) \
printf_debug(__FILE__,__FUNCTION__,__LINE__,m,##__VA_ARGS__)

#define dmalloc( size ) \
malloc_debug(size)

#define dfree( ptr ) \
free_debug(ptr)

#else	//	DEBUG

//! Print the debug message.
/*!
	Prints message to the \e stdout if DEBUG is defined else does nothing.
	\param m Message format.
*/
#define pdebug( m, ... )

//! Allocate memory.
/*!
	Allocates \a size bytes of memory.
	\param size Size to allocate.
	\note If \b DEBUG is defined then the function will count the amount of
	allocations and the total memory has been allocated. If \b DEBUG_MEMORY
	is defined then debug information will be written to the \e stdout.
	\sa dfree().
*/
#define dmalloc( size ) malloc(size)

//! Free memory.
/*!
	Frees a memory pointed by \a ptr.
	\param ptr Pointer to the memory being freed.
	\note If \b DEBUG is defined then the function will count the amount of
	allocations and the total memory has been allocated. If \b DEBUG_MEMORY
	is defined then debug information will be written to the \e stdout.
	\sa dmalloc().
*/
#define dfree( ptr ) free(ptr)

#endif	//	DEBUG

#ifdef DEBUG_FUNC

#include <stdio.h>

#define pstart() printf("### ==> %s >>>\n",__FUNCTION__);
#define pstop() printf("### <== %s <<<\n",__FUNCTION__);

#else	//	DEBUG_FUNC

//! Declare the function start.
/*!
	If \b DEBUG_FUNC is defined then a message informing that the function
	has started is sent to the \e stdout.
	\sa pstop().
*/
#define pstart()

//! Declare the function end.
/*!
	If \b DEBUG_FUNC is defined then a message informing that the function
	has stopped is sent to the \e stdout.
	\sa pstart().
*/
#define pstop()

#endif	// DEBUG_FUNC

//! Print a CHECK POINT message if DEBUG is defined.
#define _ pdebug("CHECK POINT\n");

#ifdef __cplusplus
}
#endif
#endif
