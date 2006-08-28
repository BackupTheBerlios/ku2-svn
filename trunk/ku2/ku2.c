/***************************************************************************
 *            debug.c
 *
 *  Thu Aug 17 23:14:30 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include "ku2/ecode.h"
#include "ku2/debug.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

kucode_t kucode;
unsigned int mallocs = 0;
size_t sizes = 0;

void printf_debug( const char *file, const char *func, int line, char *fmt, ... )
{
	va_list ap;
	va_start(ap, fmt);
	printf(">> %s:%d [%s] >> ", file, line, func);
	vprintf(fmt, ap);
	va_end(ap);
}

void *malloc_debug( size_t size )
{
	void *ptr = malloc(size);
	if ( ptr != NULL )
	{
		mallocs++;
		sizes += size;
	}
	#ifdef DEBUG_MEMORY
	printf("MALLOC: %p sized %d (total %d)\n", ptr, size, mallocs);
	#endif
	return ptr;
}

void free_debug( void *__ptr )
{
	free(__ptr);
	mallocs--;
	#ifdef DEBUG_MEMORY
	printf("FREE  : %p (total left %d)\n", __ptr, mallocs);
	#endif
}
