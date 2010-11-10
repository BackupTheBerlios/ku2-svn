/***************************************************************************
 *            res.c
 *
 *  Wed May 24 19:20:07 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <string.h>
#include <stdlib.h>

#include "res.h"
#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/memory.h"
#include "ku2/types.h"
#include "ds/abtree/abtree.h"

static ku_tree_t *reses, *restypes;

static kucode_t res_uload( res_t *res )
{
	res->data = res->type->control(NULL, RES_ULOAD, res->data);
	if ( res->data != NULL )
		KU_ERRQNT(KE_EXTERNAL);
	
	return KE_NONE;
}

static res_t *res_search( const char *name )
{
	res_t searchres;
	res_t *res;

	searchres.name = name;
	res = ku_abtree_search(reses, &searchres);

	return res;
}

static int res_cmpf( const void *res1, const void *res2 )
{
	return strcmp(((res_t*)res1)->name, ((res_t*)res2)->name);
}

static int res_type_cmpf( const void *rtype1, const void *rtype2 )
{
	return ((restype_t*)rtype1)->type-((restype_t*)rtype2)->type;
}

static int res_freef( void *res )
{
	if ( ((res_t*)res)->loadcnt > 0 )
		res_uload(res);
	dfree(res);

	return 0;
}

static int res_type_freef( void *rtype )
{
	dfree(rtype);
	return 0;
}

kucode_t res_init( void )
{
	pstart();

	if ( (reses = ku_abtree_create(res_cmpf, 0)) == NULL )
		KU_ERRQ_PASS();
	if ( (restypes = ku_abtree_create(res_type_cmpf, 0)) == NULL ) {
		KU_IGNORE_ERRORS(ku_abtree_free(reses, NULL));
		KU_ERRQ_PASS();
	}

	preturn KE_NONE;
}

kucode_t res_halt( void )
{
	pstart();

	ku_push_error();
	ku_abtree_free(reses, res_freef);
	ku_abtree_free(restypes, res_type_freef);
	ku_pop_error();

	preturn KE_NONE;
}

kucode_t res_assign( int type, ku_flag32_t flags, rescontrol_f control )
{
	restype_t *rtype;
	pstart();
	
	rtype = dmalloc(sizeof(restype_t));
	if ( rtype == NULL )
		KU_ERRQNT(KE_MEMORY);
	
	rtype->type = type;
	rtype->flags = flags;
	rtype->control = control;
	
	if ( ku_abtree_ins(restypes, rtype) != KE_NONE )
	{
		dfree(rtype);
		KU_ERRQ_PASS();
	}

	preturn KE_NONE;
}

kucode_t res_add( const char *path, const char *name, int type, void *param,
				 ku_flag32_t flags )
{
	res_t *res;
	restype_t searchtype, *rtype;
	pstart();
	
	searchtype.type = type;
	rtype = ku_abtree_search(restypes, &searchtype);
	if ( rtype == NULL )
		KU_ERRQNT(KE_INVALID);
	
	res = dmalloc(sizeof(res_t)+strlen(name)+strlen(path)+2);
	if ( res == NULL )
		KU_ERRQNT(KE_MEMORY);
	
	res->name = (char*)res+sizeof(res_t);
	strcpy((char*)res->name, name);
	res->path = res->name+strlen(name)+1;
	strcpy((char*)res->path, path);
	res->type = rtype;
	res->param = param;
	res->flags = flags;
	res->loadcnt = 0;
	res->data = NULL;
	
	if ( ku_abtree_ins(reses, res) != KE_NONE )
	{
		dfree(res);
		KU_ERRQ_PASS();
	}
	
	preturn KE_NONE;
}

void *res_access( const char *name )
{
	res_t *res;
	pstartp("name = %s", name);
	
	res = res_search(name);
	if ( res == NULL )
		KU_ERRQNT_V(KE_NOTFOUND, NULL);
	
	if ( (res->loadcnt == 0) || (res->type->flags&RESTYPE_UNIQ) )
	{
		//	загрузка ресурса
		res->data = res->type->control(res->path, RES_LOAD, res->param);
		if ( res->data == NULL )
			KU_ERRQNT_V(KE_EXTERNAL, NULL);
		
		res->loadcnt = 1;
	}	else
	{
		res->loadcnt++;
	}
	
	preturnp("res->data = %p", res->data) res->data;
}

void *res_access_adv( const char *name, void *param )
{
	res_t *res;
	pstart();
	
	res = res_search(name);
	if ( res == NULL )
		KU_ERRQNT_V(KE_NOTFOUND, NULL);
	
	if ( (res->loadcnt == 0) || (res->type->flags&RESTYPE_UNIQ) )
	{
		//	загрузка ресурса
		res->data = res->type->control(res->path, RES_LOAD, param);
		if ( res->data == NULL )
			KU_ERRQNT_V(KE_EXTERNAL, NULL);
		
		res->loadcnt = 1;
	}	else
	{
		res->loadcnt++;
	}
	
	preturn res->data;
}

kucode_t res_release( const char *name )
{
	res_t *res;
	pstart();
	
	res = res_search(name);
	if ( res == NULL )
		KU_ERRQNT(KE_NOTFOUND);
	
	if ( res->type->flags&RESTYPE_UNIQ )
		KU_ERRQNT(KE_INVALID);

	ku_avoid( res->loadcnt == 0 );
	if ( res->loadcnt == 1 )
	{
		//	выгрузка ресурса
		if ( res_uload(res) != KE_NONE )
			KU_ERRQ_PASS();
	}
	res->loadcnt--;
	
	preturn KE_NONE;
}
