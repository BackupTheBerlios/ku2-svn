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
#include "abtree/abtree.h"

static tree_t *reses, *restypes;

static kucode_t res_uload( res_t *res )
{
	restype_t searchtype;
	restype_t *rtype;

	searchtype.type = res->type;
	rtype = abtree_search(restypes, &searchtype);
	if ( rtype == NULL )
		KU_ERRQ(KE_INVALID);

	res->data = rtype->control(NULL, RES_ULOAD, res->data);
	if ( res->data != NULL )
		KU_ERRQ(KE_EXTERNAL);

	return KE_NONE;
}

static res_t *res_search( const char *name )
{
	res_t searchres;
	res_t *res;

	searchres.name = name;
	res = abtree_search(reses, &searchres);

	return res;
}

static int res_cmpf( void *res1, void *res2 )
{
	return strcmp(((res_t*)res1)->name, ((res_t*)res2)->name);
}

static int res_type_cmpf( void *rtype1, void *rtype2 )
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

	if ( (reses = abtree_create(res_cmpf, 0)) == NULL )
		return kucode;
	if ( (restypes = abtree_create(res_type_cmpf, 0)) == NULL )
	{
		abtree_free(reses, NULL);
		return kucode;
	}
	
	pstop();
	return KE_NONE;
}

kucode_t res_halt( void )
{
	pstart();
	
	abtree_free(reses, res_freef);
	abtree_free(restypes, res_type_freef);

	pstop();
	return KE_NONE;
}

kucode_t res_assign( int type, ku_flag32_t flags, rescontrol_f control )
{
	restype_t *rtype;
	pstart();
	
	rtype = dmalloc(sizeof(restype_t));
	if ( rtype == NULL )
		KU_ERRQ(KE_MEMORY);

	rtype->type = type;
	rtype->flags = flags;
	rtype->control = control;
	
	if ( abtree_ins(restypes, rtype) != KE_NONE )
	{
		dfree(rtype);
		return kucode;
	}
	
	pstop();
	return KE_NONE;
}

kucode_t res_add( const char *path, const char *name, int type, void *param,
				 ku_flag32_t flags )
{
	res_t *res;
	pstart();
	
	res = dmalloc(sizeof(res_t)+strlen(name)+strlen(path)+2);
	if ( res == NULL )
		KU_ERRQ(KE_MEMORY);

	res->name = (char*)res+sizeof(res_t);
	strcpy((char*)res->name, name);
	res->path = res->name+strlen(name)+1;
	strcpy((char*)res->path, path);
	res->type = type;
	res->param = param;
	res->flags = flags;
	res->loadcnt = 0;
	res->data = NULL;
	
	if ( abtree_ins(reses, res) != KE_NONE )
	{
		dfree(res);
		return kucode;
	}
	
	pstop();
	return KE_NONE;
}

void *res_access( const char *name )
{
	res_t *res;
	restype_t searchtype;
	restype_t *rtype;
	pstart();
	
	res = res_search(name);
	if ( res == NULL )
	{
		kucode = KE_NOTFOUND;
		return NULL;
	}
	
	if ( res->loadcnt == 0 )
	{
		//	загрузка ресурса
		searchtype.type = res->type;
		rtype = abtree_search(restypes, &searchtype);
		if ( rtype == NULL )
		{
			kucode = KE_INVALID;
			return NULL;
		}
		
		res->data = rtype->control(res->path, RES_LOAD, res->param);
		if ( res->data == NULL )
		{
			kucode = KE_EXTERNAL;
			return NULL;
		}
	}
	res->loadcnt++;
	
	pstop();
	return res->data;
}

kucode_t res_release( const char *name )
{
	res_t *res;
	pstart();
	
	res = res_search(name);
	if ( res == NULL )
		KU_ERRQ(KE_NOTFOUND);

	if ( res->loadcnt == 1 )
	{
		//	выгрузка ресурса
		if ( res_uload(res) != KE_NONE )
			return kucode;
	}
	res->loadcnt--;
	
	pstop();
	return KE_NONE;
}
