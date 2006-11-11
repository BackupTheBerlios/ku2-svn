/***************************************************************************
 *            debug.c
 *
 *  Thu Aug 17 23:14:30 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/types.h"
#include "ku2/gettext.h"
#include "io/log/log.h"

kucode_t kucode;

static uint mallocs = 0;
static size_t sizes = 0;
static int ku_func_debug_is_on = 0;

void printf_debug( const char *file, const char *func, int line, char *fmt, ... )
{
	va_list ap;
	va_start(ap, fmt);
	printf(">> %s:%d [%s] >> ", file, line, func);
	vprintf(fmt, ap);
	va_end(ap);
}

void pavoid_debug( const char *file, const char *func, int line )
{
	plog(gettext("Not avoided expression >> %s: %d [%s]\n"), file, line, func);
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

void memory_stat( void )
{
	plog(gettext("Memory status:\n" \
		"\tbytes allocated: %d\n" \
		"\tblocks: %u.\n"), sizes, mallocs);
}

void func_debug( int status )
{
	ku_func_debug_is_on = status;
}

void pstart_debug( const char *func )
{
	if ( ku_func_debug_is_on )
		printf("### ==> %s >>>\n", func);
}

void pstop_debug( const char *func )
{
	if ( ku_func_debug_is_on )
		printf("### <== %s <<<\n", func);
}
