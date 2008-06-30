/*
		vspace.c
		Sat Jul 14 21:29:26 2007

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2007
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#include "dp/var/vlist.h"
#include "dp/var/var.h"
#include "dp/var/vspace.h"
#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/memory.h"

#define VSPACE_MAX_NODE_LEN 64

static int vspace_freef( void *space )
{
	vspace_undef(space);
	return 0;
}

static int vspace_compf( const void *a, const void *b )
{
	return strcmp(((vspace_t*)a)->name, ((vspace_t*)b)->name);
}

static int var_freef( void *var )
{
	var_undef(var);
	return 0;
}

static int var_compf( const void *a, const void *b )
{
	return strcmp(((var_t*)a)->name, ((var_t*)b)->name);
}

vspace_t *vspace_define( const char *name )
{
	vspace_t *space;
	pstart();
	
	space = dmalloc(sizeof(vspace_t)+strlen(name)+1);
	if ( space == NULL )
		KU_ERRQ_VALUE(KE_MEMORY, NULL);
	
	space->name = (char*)space+sizeof(vspace_t);
	strcpy(space->name, name);
	space->vars = space->spaces = NULL;
	
	preturn space;
}

kucode_t vspace_undef( vspace_t *space )
{
	pstart();
	
	if ( space->vars != NULL )
		abtree_free(space->vars, var_freef);
	if ( space->spaces != NULL )
		abtree_free(space->spaces, vspace_freef);
	dfree(space);
	
	preturn KE_NONE;
}

kucode_t vspace_addv( vspace_t *space, const char *name, const char *val_types, ... )
{
	var_t *var;
	va_list ap;
	pstart();
	
	va_start(ap, val_types);
	var = var_define_v(name, val_types, ap);
	va_end(ap);
	if ( var == NULL )
		{ preturn KU_GET_ERROR(); }
	
	if ( space->vars == NULL )
	{
		space->vars = abtree_create(var_compf, 0);
		if ( space->vars == NULL )
			{ preturn KU_GET_ERROR(); }
		if ( abtree_ins(space->vars, var) != KE_NONE )
		{
			var_undef(var);
			abtree_free(space->vars, NULL);
			space->vars = NULL;
			preturn KU_GET_ERROR();
		}
	}	else
	if ( abtree_ins(space->vars, var) != KE_NONE )
	{
		var_undef(var);
		preturn KU_GET_ERROR();
	}
	
	preturn KE_NONE;
}

kucode_t vspace_addv_l( vspace_t *space, const char *name, vlist_t *vlist )
{
	var_t *var;
	pstart();
	
	var = var_define_l(name, vlist);
	if ( var == NULL )
		{ preturn KU_GET_ERROR(); }
	
	if ( space->vars == NULL )
	{
		space->vars = abtree_create(var_compf, 0);
		if ( space->vars == NULL )
			{ preturn KU_GET_ERROR(); }
		if ( abtree_ins(space->vars, var) != KE_NONE )
		{
			var_undef(var);
			abtree_free(space->vars, NULL);
			space->vars = NULL;
			preturn KU_GET_ERROR();
		}
	}	else
	if ( abtree_ins(space->vars, var) != KE_NONE )
	{
		var_undef(var);
		preturn KU_GET_ERROR();
	}
	
	preturn KE_NONE;
}

kucode_t vspace_adds( vspace_t *space, const char *name )
{
	vspace_t *new_space;
	pstart();
	
	new_space = vspace_define(name);
	if ( new_space == NULL )
		{ preturn KU_GET_ERROR(); }
	
	if ( space->spaces == NULL )
	{
		space->spaces = abtree_create(vspace_compf, 0);
		if ( space->spaces == NULL )
			{ preturn KU_GET_ERROR(); }
		if ( abtree_ins(space->spaces, new_space) != KE_NONE )
		{
			vspace_undef(new_space);
			abtree_free(space->spaces, NULL);
			space->spaces = NULL;
			preturn KU_GET_ERROR();
		}
	}	else
	if ( abtree_ins(space->spaces, new_space) != KE_NONE )
	{
		vspace_undef(new_space);
		preturn KU_GET_ERROR();
	}
	
	preturn KE_NONE;
}

static vspace_t *vspace_get_leaf( vspace_t *space, const char *path, char **leaf )
{
	char *p;
	const char *cur = path;
	static char buf[VSPACE_MAX_NODE_LEN];
	vspace_t search_space;
	pstart();
	
	cur = path;
	search_space.name = buf;
	for (;;)
	{	
		p = buf;
		while ( (*cur != '/') && (*cur) )
		{
			*(p++) = *(cur++);
			if ( p-buf >= VSPACE_MAX_NODE_LEN )
				KU_ERRQ_VALUE(KE_FULL, NULL);
		}
		*p = 0;
		if ( *cur == 0 )
			break;
		cur++;
	
		space = ku_abtree_search(space->spaces, &search_space);
		if ( space->spaces == NULL )
			KU_ERRQ_VALUE(KE_NOTFOUND, NULL);
	}
	
	*leaf = buf;
	preturn space;
}

const var_t *vspace_getv( vspace_t *space, const char *path )
{
	var_t *var, search_var;
	pstart();
	
	space = vspace_get_leaf(space, path, &search_var.name);
	if ( space == NULL )
		{ preturn NULL; }
	
	if ( space->vars == NULL )
	{
		KU_SET_ERROR(KE_NOTFOUND);
		preturn NULL;
	}
	var = ku_abtree_search(space->vars, &search_var);
	if ( var == NULL )
		{ preturn NULL; }
	
	preturn var;
}

vspace_t *vspace_gets( vspace_t *space, const char *path )
{
	vspace_t search_space;
	pstart();
	
	space = vspace_get_leaf(space, path, &search_space.name);
	if ( space == NULL )
		{ preturn NULL; }
	
	if ( space->spaces == NULL )
	{
		KU_SET_ERROR(KE_NOTFOUND);
		preturn NULL;
	}
	space = ku_abtree_search(space->spaces, &search_space);
	if ( space == NULL )
		{ preturn NULL; }
	
	preturn space;
}
