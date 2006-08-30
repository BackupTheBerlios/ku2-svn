/***************************************************************************
 *            gui.c
 *
 *  Fri Jun  2 18:26:00 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include "gui.h"
#include "ku2/debug.h"
#include "ku2/ecode.h"
#include "ku2/memory.h"
#include "ku2/types.h"
#include "list/list.h"
#include "abtree/abtree.h"

//! Comparing function.
static int gui_compf( void *obj1, void *obj2 )
{
	return (int*)obj1-(int*)obj2;
}

//! Freeing function.
static int gui_freef( void *obj )
{
	dfree(obj);
	return 0;
}

static tree_t *gui_objects;

static gui_obj_t *obj_root;

kucode_t gui_init( void )
{
	pstart();

	gui_objects = abtree_create(gui_compf, 0);
	if ( gui_objects == NULL )
		return kucode;

	pstop();
	return KE_NONE;
}

kucode_t gui_halt( void )
{
	pstart();

	abtree_free(gui_objects, gui_freef);

	pstop();
	return KE_NONE;
}

gui_obj_t *gui_obj_create( gui_load_f loadf, uint widget_sz )
{
	gui_obj_t *obj;
	pstart();

	obj = dmalloc(sizeof(gui_obj_t)+widget_sz);
	if ( obj = NULL )
	{
		kucode = KE_MEMORY;
		return NULL;
	}

	obj->parent = NULL;
	obj->children = NULL;
	obj->status = GUI_NOTLOADED;
	obj->widget_sz = widget_sz;
	obj->loadf = loadf;
	obj->widget = (int8_t*)obj+sizeof(gui_obj_t);

	if ( loadf(obj) != KE_NONE )
	{
		dfree(obj);
		return NULL;
	}

	if ( abtree_ins(gui_objects, obj) != KE_NONE )
	{
		dfree(obj);
		return NULL;
	}

	pstop();
	return obj;
}

#if 0
gui_obj_t *gui_obj_clone( gui_obj_t *obj )
{

}
#endif

kucode_t gui_obj_delete( gui_obj_t *obj )
{
	pstart();
	gui_freef(obj);
	pstop();
	return KE_NONE;
}

