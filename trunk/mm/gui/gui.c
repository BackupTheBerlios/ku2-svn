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
	if ( ((gui_obj_t*)obj)->status > GUI_NOTLOADED )
		((gui_obj_t*)obj)->uloadf(obj);
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

gui_obj_t *gui_obj_create( gui_load_f loadf, uint widget_sz, ku_flag32_t flags )
{
	gui_obj_t *obj;
	pstart();

	obj = dmalloc(sizeof(gui_obj_t)+widget_sz);
	if ( obj == NULL )
	{
		kucode = KE_MEMORY;
		return NULL;
	}

	obj->parent = NULL;
	obj->children = NULL;
	obj->status = GUI_NOTLOADED;
	obj->flags = flags;
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
	static int recursion_lvl = 0;
	pstart();
	
	recursion_lvl++;
	
	if ( obj->status > GUI_NOTLOADED )
	{
		// удаляем подобъект
		obj->uloadf(obj);
	}
	
	// удаляем потомков
	if ( obj->children )
	{
		if ( dl_list_first(obj->children) == KE_NONE )
			do gui_obj_delete(dl_list_cur(obj->children));
				while ( !dl_list_islast(obj->children) );
		dl_list_free(obj->children, NULL);
	}
	
	// исключаем из потомков предка
	if ( (recursion_lvl == 1) && obj->parent )
	{
		if ( dl_list_search(obj->parent->children, obj) == KE_NONE )
			dl_list_rem(obj->parent->children, NULL);
		if ( (obj->parent->children->size == 0) &&
			((obj->parent->flags&GUI_KEEPCHILDEN) != GUI_KEEPCHILDEN) )
		{
			dl_list_free(obj->parent->children, NULL);
			obj->parent->children = NULL;
		}
	}
	
	dfree(obj);
	recursion_lvl--;
	
	pstop();
	return KE_NONE;
}
