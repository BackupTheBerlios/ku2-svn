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
#include "ku2/memory.h"

#include "ds/abtree/abtree.h"

#include "dp/var/var.h"

static tree_t *var_tree;

static int var_cmpf( const void *a, const void *b )
{
	return strcmp((char*)a+sizeof(var_t), (char*)b+sizeof(var_t));
}

static int var_freef( void *var )
{
	dfree(var);
	return 0;
}

kucode_t var_init( ku_flag32_t flags __attribute__((unused)))
{
	pstart();
	
	var_tree = abtree_create(var_cmpf, 0);
	if ( var_tree == NULL )
		preturn kucode;
	
	preturn KE_NONE;
}

void var_done( void )
{
	pstart();
	
	abtree_free(var_tree, var_freef);
	
	pstop();
}

kucode_t var_define( const char *name )
{
	var_t *var;
	pstart();
	
	var = dmalloc(sizeof(var_t)+(strlen(name)+1)*sizeof(char));
	if ( var == NULL )
		KU_ERRQ(KE_MEMORY);
	strcpy((char*)var+sizeof(var_t), name);
	var->size = 0;
	var->data = NULL;
	
	if ( abtree_ins(var_tree, var) != KE_NONE )
	{
		dfree(var);
		preturn kucode;
	}
	
	preturn KE_NONE;
}

kucode_t var_define_val( const char *name, uint size, const void *data )
{
	var_t *var;
	uint name_len = strlen(name)+1;
	pstart();
	
	var = dmalloc(sizeof(var_t)+name_len*sizeof(char)+size);
	if ( var == NULL )
		KU_ERRQ(KE_MEMORY);
	strcpy((char*)var+sizeof(var_t), name);
	var->size = size;
	var->data = (char*)var+sizeof(var_t)+name_len;
	memcpy(var->data, data, size);
	
	if ( abtree_ins(var_tree, var) != KE_NONE )
	{
		dfree(var);
		preturn kucode;
	}
	
	preturn KE_NONE;
}

kucode_t var_redefine( const char *name, uint size, const void *data )
{
	var_t *var;
	uint name_len = strlen(name)+1;
	pstart();
	
	var = dmalloc(sizeof(var_t)+name_len*sizeof(char)+size);
	if ( var == NULL )
		KU_ERRQ(KE_MEMORY);
	strcpy((char*)var+sizeof(var_t), name);
	var->size = size;
	var->data = (char*)var+sizeof(var_t)+name_len;
	memcpy(var->data, data, size);
	
	if ( abtree_replace(var_tree, var, var, var_freef) != KE_NONE )
	{
		dfree(var);
		preturn kucode;
	}
	
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
