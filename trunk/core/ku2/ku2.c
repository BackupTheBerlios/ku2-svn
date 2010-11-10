/*
		ku2.c

	This file is the part of Kane Utilities 2.
	See licensing agreement in a root directory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2008
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

/*!
 * \file
 * \mainpage
 * It is a documentation for Kane Utilities 2.
 *
 * Current version is 2.0.
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/types.h"
#include "ku2/gettext.h"
#include "ku2/memory.h"
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
	plog(gettext(ESC_BRED("Not avoided expression >> %s: %d [%s]")"\n"),
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
	printf(ESC_BLUE("MALLOC : %p sized %ld (total %d)")"\n", ptr,
	       (long int)size, mallocs);
	#endif
	return ptr;
}

void ku_free_debug( void *__ptr )
{
	free(__ptr);
	mallocs--;
	#ifdef DEBUG_MEMORY
	printf(ESC_BLUE("FREE   : %p (total left %d)")"\n", __ptr, mallocs);
	#endif
}

void *ku_realloc_debug( void *__ptr, size_t size )
{
	void *ptr = realloc(__ptr, size);
	#ifdef DEBUG_MEMORY
	printf(ESC_BLUE("REALLOC: %p changed to %p sized %ld (total %d)")"\n",
	       __ptr, ptr, (long int)size, mallocs);
	#endif
	return ptr;
}

void ku_memory_stat( void )
{
	plog(gettext(ESC_BBLUE("Memory status:\n" \
		"\tbytes allocated: %d\n" \
		"\tblocks: %u.")"\n"), sizes, mallocs);
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
			printf(ESC_BYELLOW("### ==> %s >>>")"\n", func); else
		{
			va_list ap;
			va_start(ap, fmt);
			printf(ESC_BYELLOW("### ==> %s( "), func);
			vprintf(fmt, ap);
			printf(ESC_BYELLOW(" ) >>>")"\n");
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
			printf(ESC_RED("NEGATIVE LEVEL ERROR ### <== %s <<<")"\n", func);
			ku_func_lvl = 0;
			return;
		}
		for ( i = 0; i < ku_func_lvl; i++ )
			printf("   ");
		if ( fmt == NULL )
			printf(ESC_YELLOW("### <== %s <<<")"\n", func); else
		{
			va_list ap;
			va_start(ap, fmt);
			printf(ESC_YELLOW("### <== %s( "), func);
			vprintf(fmt, ap);
			printf(ESC_YELLOW(" ) <<<")"\n");
			va_end(ap);
		}
	}
}


/*******************************************************************************
	Error handling (ecode.h).
*******************************************************************************/

//! Last error description.
static KU_THREAD_LOCAL(struct KU_STRUCT_ERRDESC)
{
	kucode_t ecode;        //!< Error code.
	char *text;            //!< Error text.
	int memerr;            //!< Memory error (during setting an error) indicator.
}	ku_errdesc;

//! Initialise mt_errdesc.
static void KU_CONSTRUCTOR(ku_errdesc_init( void ))
{
	ku_errdesc.ecode = KE_NONE;
	ku_errdesc.text = NULL;
	ku_errdesc.memerr = 0;
}

//! Clean-up ku_errdesc.
static void KU_DESTRUCTOR(ku_errdesc_clean( void ))
{
	if ( (ku_errdesc.text != NULL) && !ku_errdesc.memerr )
		dfree(ku_errdesc.text);
	dlogmemstat();
}

kucode_t ku_serror( kucode_t ecode, const char *etext )
{
	ku_errdesc.ecode = ecode;

	if ( etext != NULL )
	{
		int __len = strlen(etext) + 1;
		char *__text;

		__text = drealloc(ku_errdesc.text, __len);
		if ( __text == NULL ) // Failed to allocate memory for error text:
		{
			// Freeing the old error text:
			if ( (ku_errdesc.text != NULL) && !ku_errdesc.memerr )
				dfree(ku_errdesc.text);

			ku_errdesc.ecode = KE_MEMORY;
			ku_errdesc.text = _("Failed to allocate memory for error message!");
			ku_errdesc.memerr = 1;
		}	else // realloc() succeeded:
		{
			memcpy(__text, etext, __len);
			ku_errdesc.text = __text;
			ku_errdesc.memerr = 0;
		}
	}	else
	{
		// Freeing the old error text:
		if ( (ku_errdesc.text != NULL) && !ku_errdesc.memerr )
			dfree(ku_errdesc.text);

		ku_errdesc.text = NULL;
		ku_errdesc.memerr = 0;
	}

	return ecode;
}

void ku_rerror( void )
{
	ku_serror(KE_NONE, NULL);
}

const char *ku_gerrtx( void )
{
	if ( ku_errdesc.text == NULL )
	{
		return _("(error text was not set)");
	}	else
	{
		return ku_errdesc.text;
	}
}

kucode_t ku_gerrcode( void )
{
	return ku_errdesc.ecode;
}
