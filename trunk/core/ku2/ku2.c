/*
 *	core:ku2:ku2.c
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2008+
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

/*!
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


/*******************************************************************************
	Debugging (debug.h).
*******************************************************************************/

void ku_printf_debug( const char *file, const char *func, int line, const char *fmt, ... )
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
	     file, line, func);
}

typedef
struct KU_STRUCT_MEMORYDESC
{
	size_t size;
	struct KU_STRUCT_MEMORYDESC *next;
	char ptr[0];
} ku_memorydesc_t;

//! Debugging data.
static KU_THREAD_LOCAL(struct KU_STRUCT_DEBUGDESC)
{
	// Memory related:
	uint mallocs;
	size_t malloc_size;
	ku_memorydesc_t *memory_head;

	int func_debug_is_on;
	uint func_lvl;
}	ku_debugdesc;

//! Initialise mt_debugdesc.
static void KU_CONSTRUCTOR(ku_debugdesc_init( void ))
{
	printf(ESC_BBLUE("Constructing ku_debugdesc_init...")"\n");
	ku_debugdesc.mallocs = 0;
	ku_debugdesc.malloc_size = 0;
	ku_debugdesc.memory_head = NULL;

	ku_debugdesc.func_debug_is_on = 0;
	ku_debugdesc.func_lvl = 0;
}

//! Clean-up ku_errdesc.
static void KU_DESTRUCTOR(ku_debugdesc_clean( void ))
{
	dlogmemstat();
}

void *ku_malloc_debug( size_t size )
{
	ku_memorydesc_t *ptr = malloc(sizeof(ku_memorydesc_t) + size);
	if ( ptr != NULL ) {
		ptr->size = size;
		ptr->next = ku_debugdesc.memory_head;
		ku_debugdesc.memory_head = ptr;
		ku_debugdesc.mallocs++;
		ku_debugdesc.malloc_size += size;
	}

	if ( getenv("KU_TRACE_MEMORY") != NULL ) {
		printf(ESC_BLUE("MALLOC : %p sized %ld " \
		                "(total %u with %ld bytes)")"\n", ptr->ptr,
		       (long int)size,
		       ku_debugdesc.mallocs, (long int)ku_debugdesc.malloc_size);
	}

	return ptr->ptr;
}

static ku_memorydesc_t *ku_find_alloced_ptr( void *ptr, ku_memorydesc_t **prev )
{
	ku_memorydesc_t *desc = ku_debugdesc.memory_head;

	// If the looked pointer is the first one:
	if ( desc->ptr == ptr ) {
		*prev = NULL;
		return desc;
	}

	ku_memorydesc_t *prev_desc;
	do {
		prev_desc = desc;
		desc = desc->next;
	} while ( desc && (desc->ptr != ptr) );

	*prev = prev_desc;
	return desc;
}

void ku_free_debug( void *ptr )
{
	ku_memorydesc_t *desc, *prev;
	desc = ku_find_alloced_ptr(ptr, &prev);

	int trace = (getenv("KU_TRACE_MEMORY") != NULL);
	if ( desc == NULL ) {
		if ( trace ) {
			printf(ESC_BLUE("FREE   : %p was not allocated")"\n",
			       ptr);
		}
		free(ptr);

	} else {
		ku_debugdesc.mallocs--;
		ku_debugdesc.malloc_size -= desc->size;
		if ( trace ) {
			printf(ESC_BLUE("FREE   : %p sized %ld (total left %u " \
			                "with size %ld)")"\n",
			       desc->ptr, (long int)desc->size,
			       ku_debugdesc.mallocs, (long int)ku_debugdesc.malloc_size);
		}

		// Remove the description from the list:
		if ( prev ) {
			prev->next = desc->next;
		} else {
			ku_debugdesc.memory_head = desc->next;
		}

		// Finish it:
		free(desc);
	}
}

void *ku_realloc_debug( void *ptr, size_t size )
{
	ku_memorydesc_t *desc, *prev;
	desc = ku_find_alloced_ptr(ptr, &prev);

	int trace = (getenv("KU_TRACE_MEMORY") != NULL);
	if ( desc == NULL ) {
		if ( trace ) {
			printf(ESC_BLUE("REALLOC: %p was not allocated")"\n",
			       ptr);
		}
		return realloc(ptr, size);
	}

	ku_memorydesc_t *old_desc = desc;
	desc = realloc(desc, sizeof(ku_memorydesc_t) + size);
	if ( desc == NULL ) {
		if ( trace ) {
			printf(ESC_BLUE("REALLOC: %p failed")"\n", desc->ptr);
		}
		return NULL;
	}

	ku_debugdesc.malloc_size += size - desc->size;
	if ( trace ) {
		printf(ESC_BLUE("REALLOC: %p (%ld b) changed to %p (%ld b) " \
		                "(total %u with %ld bytes)")"\n",
		       old_desc->ptr, (long int)desc->size,
		       desc->ptr, (long int)size,
		       ku_debugdesc.mallocs, (long int)ku_debugdesc.malloc_size);
	}
	desc->size = size;

	// If the memory was moved:
	if ( old_desc != desc ) {
		if ( prev ) {
			prev->next = desc;
		} else {
			ku_debugdesc.memory_head = desc;
		}
	}

	return desc->ptr;
}

void ku_memory_stat( void )
{
	printf(ESC_BBLUE("ku2core: not freed memory status:\n" \
	                 "\tbytes allocated: %u\n" \
	                 "\tblocks: %u.")"\n",
	       ku_debugdesc.malloc_size,
	       ku_debugdesc.mallocs);
}

void ku_func_debug( int status )
{
	ku_debugdesc.func_debug_is_on = status;
}

void ku_pstart_debug( const char *func, const char *fmt, ... )
{
	if ( ku_debugdesc.func_debug_is_on )
	{
		uint i;
		for ( i = 0; i < ku_debugdesc.func_lvl; i++ )
			printf("   ");
		ku_debugdesc.func_lvl++;
		if ( fmt == NULL )
			printf(ESC_BCYAN("### ==> %s >>>")"\n", func); else
		{
			va_list ap;
			va_start(ap, fmt);
			printf(ESC_BCYAN("### ==> %s( "), func);
			vprintf(fmt, ap);
			printf(ESC_BCYAN(" ) >>>")"\n");
			va_end(ap);
		}
	}
}

void ku_pstop_debug( const char *func, const char *fmt, ... )
{
	if ( ku_debugdesc.func_debug_is_on )
	{
		uint i;
		if ( --ku_debugdesc.func_lvl == (uint)-1 )
		{
			printf(ESC_RED("NEGATIVE LEVEL ERROR ### <== %s <<<")"\n", func);
			ku_debugdesc.func_lvl = 0;
			return;
		}
		for ( i = 0; i < ku_debugdesc.func_lvl; i++ )
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

const char *ku_gerrname( void )
{
	static const char *const names[] = {
	    "KE_NONE (no error)",
	    "KE_NOIMPLEM (feature is not implemented yet)",
	    "KE_SIGNAL (alternative signal message)",
	    "KE_ASSERT (assertion has failed)",
	    "KE_MEMORY (memory allocation error)",
	    "KE_EMPTY (data container is empty)",
	    "KE_FULL (data container is full)",
	    "KE_NOTFOUND (data was not found)",
	    "KE_DOUBLE (data already exists)",
	    "KE_IO (input/output error)",
	    "KE_SYNTAX (syntax error)",
	    "KE_EXTERNAL (external error)",
	    "KE_INVALID (invalid data)",
	    "KE_LIBRARY (external library error)",
	    "KE_UNDERFLOW (underflow detected)",
	    "KE_OVERFLOW (overflow detected)",
	    "KE_RANGE (out of range)"
	};
	// Ensure that all error codes are represented here:
	char constraint[sizeof(names)/sizeof(names[0]) == _KE_LAST_ENUM ? 0 : -1];
	(void)constraint;
	return names[ku_errdesc.ecode];
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

void ku_push_error( void )
{
}

void ku_pop_error( void )
{
}
