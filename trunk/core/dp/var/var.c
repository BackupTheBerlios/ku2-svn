/*
	This file is the part of Kane Utilities 2.
	See licencing agreement in a root directory for more details.

	Copyright J. Anton, 2007
*/

#include <stdlib.h>
#include <string.h>

#include "ku2/ecode.h"
#include "ku2/types.h"
#include "ku2/debug.h"

#include "ds/abtree/abtree.h"

#include "dp/var/var.h"

static tree_t *var_tree;

static int var_cmpf( const void *a, const void *b )
{
	return strcmp(((var_t*)a)->name, ((var_t*)b)->name);
}

kucode_t var_init( ku_flag32_t flags __attribute__((unused)))
{
	pstart();
	
	var_tree = abtree_create(var_cmpf, 0);
	if ( var_tree == NULL )
		preturn kucode;
	
	pstop();
	preturn KE_NONE;
}

void var_done( void )
{
	pstart();
	pstop();
}

kucode_t var_define( const char *name )
{
	pstart();
	pstop();
	preturn KE_NONE;
}

kucode_t var_define_val( const char *name, uint size, const void *data )
{
	pstart();
	pstop();
	preturn KE_NONE;
}

kucode_t var_redefine( const char *name, uint size, const void *data )
{
	pstart();
	pstop();
	preturn KE_NONE;
}

kucode_t var_undef( const char *name )
{
	pstart();
	pstop();
	preturn KE_NONE;
}

int var_defined( const char *name )
{
	pstart();
	pstop();
	preturn 0;
}

kucode_t var_get( const char *name, void *data )
{
	pstart();
	pstop();
	preturn KE_NONE;
}

void *var_addr( const char *name )
{
	pstart();
	pstop();
	preturn NULL;
}
