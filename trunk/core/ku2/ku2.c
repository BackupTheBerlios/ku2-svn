/*
		ku2.c
		Mon Jun 23 07:57:58 2008

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2008
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/types.h"
#include "ku2/gettext.h"
#include "other/other.h"
#include "io/log/log.h"

__thread kucode_t kucode;

static uint mallocs = 0;
static size_t sizes = 0;
static int ku_func_debug_is_on = 0;
static uint ku_func_lvl = 0;

void ku_printf_debug( const char *file, const char *func, int line, char *fmt, ... )
{
	va_list ap;
	va_start(ap, fmt);
	printf(ESC_GREEN(">> %s:%d [%s] >> "), file, line, func);
	vprintf(fmt, ap);
	va_end(ap);
}

void ku_pavoid_debug( const char *file, const char *func, int line )
{
	plog(gettext(ESC_BRED("Not avoided expression >> %s: %d [%s]\n")),
		 file,
		 line,
		 func);
}

void *ku_malloc_debug( size_t size )
{
	void *ptr = malloc(size);
	if ( ptr != NULL )
	{
		mallocs++;
		sizes += size;
	}
	#ifdef DEBUG_MEMORY
	printf(ESC_BLUE("MALLOC : %p sized %d (total %d)\n"), ptr, size, mallocs);
	#endif
	return ptr;
}

void ku_free_debug( void *__ptr )
{
	free(__ptr);
	mallocs--;
	#ifdef DEBUG_MEMORY
	printf(ESC_BLUE("FREE   : %p (total left %d)\n"), __ptr, mallocs);
	#endif
}

void *ku_realloc_debug( void *__ptr, size_t size )
{
	void *ptr = realloc(__ptr, size);
	#ifdef DEBUG_MEMORY
	printf(ESC_BLUE("REALLOC: %p changed to %p sized %d (total %d)\n"),
		   __ptr, ptr, size, mallocs);
	#endif
	return ptr;
}

void ku_memory_stat( void )
{
	plog(gettext(ESC_BBLUE("Memory status:\n" \
		"\tbytes allocated: %d\n" \
		"\tblocks: %u.\n")), sizes, mallocs);
}

void ku_func_debug( int status )
{
	ku_func_debug_is_on = status;
}

void ku_pstart_debug( const char *func, char *fmt, ... )
{
	if ( ku_func_debug_is_on )
	{
		uint i;
		for ( i = 0; i < ku_func_lvl; i++ )
			printf("   ");
		ku_func_lvl++;
		if ( fmt == NULL )
			printf(ESC_BYELLOW("### ==> %s >>>\n"), func); else
		{
			va_list ap;
			va_start(ap, fmt);
			printf(ESC_BYELLOW("### ==> %s( "), func);
			vprintf(fmt, ap);
			printf(ESC_BYELLOW(" ) >>>\n"));
			va_end(ap);
		}
	}
}

void ku_pstop_debug( const char *func, char *fmt, ... )
{
	if ( ku_func_debug_is_on )
	{
		uint i;
		if ( --ku_func_lvl == (uint)-1 )
		{
			printf(ESC_RED("NEGATIVE LEVEL ERROR ### <== %s <<<\n"), func);
			ku_func_lvl = 0;
			return;
		}
		for ( i = 0; i < ku_func_lvl; i++ )
			printf("   ");
		if ( fmt == NULL )
			printf(ESC_YELLOW("### <== %s <<<\n"), func); else
		{
			va_list ap;
			va_start(ap, fmt);
			printf(ESC_YELLOW("### <== %s( "), func);
			vprintf(fmt, ap);
			printf(ESC_YELLOW(" ) <<<\n"));
			va_end(ap);
		}
	}
}

kucode_t ku_set_error( kucode_t __ecode )
{
	return kucode = __ecode;
}

kucode_t ku_get_error( void )
{
	return kucode;
}
