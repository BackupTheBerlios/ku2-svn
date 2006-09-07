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
#include "log/log.h"
#include "ku2/gettext.h"

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

//! Exclude the object from parent`s children list.
static void gui_excl_from_parent( gui_obj_t *obj )
{
	if ( dl_list_search(obj->parent->children, obj) == KE_NONE )
		dl_list_rem(obj->parent->children, NULL);
	if ( (obj->parent->children->size == 0) &&
		((obj->parent->flags&GUI_KEEPCHILDREN) != GUI_KEEPCHILDREN) )
	{
		dl_list_free(obj->parent->children, NULL);
		obj->parent->children = NULL;
	}
}

static kucode_t gui_ins_to_children( gui_obj_t *parent, gui_obj_t *child )
{
	if ( parent->children == NULL )
	{
		parent->children = dl_list_create(gui_compf, 0);
		if ( parent->children == NULL )
			return kucode;
	}
	if ( dl_list_ins_last(parent->children, child) != KE_NONE )
		return kucode;

	return KE_NONE;
}

static tree_t *gui_objects;
static uint gui_lastid = 0;

static gui_obj_t *obj_root;
static gui_obj_t *obj_mon;

kucode_t gui_init( void )
{
	pstart();

	gui_objects = abtree_create(gui_compf, 0);
	if ( gui_objects == NULL )
		return kucode;

	obj_root = NULL;
	obj_mon = NULL;
	
	plog(gettext("GUI engine has been initialized.\n"));
	pstop();
	return KE_NONE;
}

kucode_t gui_halt( void )
{
	pstart();

	abtree_free(gui_objects, gui_freef);

	plog(gettext("GUI engine has been halted.\n"));
	pstop();
	return KE_NONE;
}

gui_obj_t *gui_obj_create( gui_load_f initf, uint widget_sz, ku_flag32_t flags )
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
	obj->initf = initf;
	obj->widget = (int8_t*)obj+sizeof(gui_obj_t);

	if ( initf(obj) != KE_NONE )
	{
		dfree(obj);
		return NULL;
	}

	if ( abtree_ins(gui_objects, obj) != KE_NONE )
	{
		dfree(obj);
		return NULL;
	}

	obj->id = gui_lastid++;

	plog(gettext("Object \"%s\" %u is created!\n"), (char*)obj->widget, obj->id);
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
		gui_excl_from_parent(obj);
	}
	
	plog(gettext("Object \"%s\" %u is deleted!\n"), (char*)obj->widget, obj->id);
	dfree(obj);
	recursion_lvl--;
	
	pstop();
	return KE_NONE;
}

kucode_t gui_move( gui_obj_t *obj, gui_obj_t *host, int x, int y, int w, int h )
{
	gui_obj_t *tobj;
	pstart();

	if ( (obj->parent != host) && host )
	{
		// меняется иерархия объектов
		if ( gui_ins_to_children(host, obj) != KE_NONE )
			return kucode;
		if ( obj->parent )
		{
			gui_excl_from_parent(obj);
			gui_draw(obj->parent, obj->x, obj->y, obj->width, obj->height);
		}
		obj->parent = host;

		// поиск абсолютных координат
		obj->rx = x;
		obj->ry = y;
		for ( tobj = obj->parent; tobj; tobj = tobj->parent )
		{
			obj->rx += tobj->x;
			obj->ry += tobj->y;
		}
	}	else
	if ( host == NULL )
	{
		// объект становится корневым
		obj_root = obj;
		obj->parent = NULL;

		obj->rx = x;
		obj->ry = y;
	}

	obj->x = x;
	obj->y = y;
	if ( w+h != 0 )
	{
		obj->width = w;
		obj->height = h;

		if ( obj->dim(obj) != KE_NONE )
			plog(gettext("Note: a widget failed to change its dimension: %d\n"), kucode);
	}

	plog(gettext("Object \"%s\" %u was moved!\n"), (char*)obj->widget, obj->id);
	pstop();
	return KE_NONE;
}

kucode_t gui_set( gui_obj_t *obj, int param, void *data )
{
	pstart();

	if ( obj->set(obj, param, data) != KE_NONE )
		return kucode;

	pstop();
	return KE_NONE;
}

kucode_t gui_get( gui_obj_t *obj, int param, void *data )
{
	pstart();

	if ( obj->get(obj, param, data ) != KE_NONE )
		return kucode;

	pstop();
	return KE_NONE;
}

kucode_t gui_ch_status( gui_obj_t *obj, gui_status_t status )
{
	pstart();

	if ( obj->status != status )
	switch ( status )
	{
		case GUI_NOTLOADED:
		{
			// выгрузить объект
			if ( obj->uloadf(obj) != KE_NONE )
				return kucode;
			break;
		}
		case GUI_ENABLED:
		{
			// загрузить объект, если он не загружен
			// сделать его доступным
			if ( obj->status == GUI_NOTLOADED )
			{
				if ( obj->loadf(obj) != KE_NONE )
					return kucode;
			}
			if ( obj->enable(obj) != KE_NONE )
				return kucode;
			break;
		}
		case GUI_DISABLED:
		{
			// сделать объект недоступным, но видимым
			if ( obj->disable(obj) != KE_NONE )
				return kucode;
			break;
		}
		case GUI_HIDDEN:
		{
			// скрыть объект
			if ( obj->hide(obj) != KE_NONE )
				return kucode;
		}
	}

	obj->status = status;

	pstop();
	return KE_NONE;
}

kucode_t gui_draw( gui_obj_t *obj, int x, int y, int w, int h )
{
	pstart();

	if ( obj->draw(obj, 0, 0, 0, 0) != KE_NONE )
		return kucode;

	if ( obj->children )
	{
		if ( dl_list_first(obj->children) == KE_NONE )
			do if ( gui_draw(dl_list_cur(obj->children), 0, 0, 0, 0) != KE_NONE )
				return kucode; while ( !dl_list_islast(obj->children) );
	}

	pstop();
	return KE_NONE;
}

static gui_obj_t *gui_search_by_coord( int x, int y )
{
	gui_obj_t *obj = obj_root;
	
	if ( (!obj) || (obj->rx > x) || (obj->rx+obj->width < x) || \
		(obj->ry > y) || (obj->ry+obj->height < y ) )
		return NULL;
	
	while ( obj->children && (obj->children->size > 0) )
	{
		dl_list_last(obj->children);
		for (;;)
		{
			gui_obj_t *nobj = dl_list_prev(obj->children);
			if ( (nobj->rx <= x) && (nobj->rx+nobj->width >= x) && \
				(nobj->ry <=y ) && (nobj->ry+nobj->height >= y) )
			{
				obj = nobj;
				break;
			}
			if  ( dl_list_isfirst(obj->children) )
				return obj;
		}
	}
	
	return obj;
}

gui_event_st gui_process( SDL_Event *event )
{
	gui_obj_t *obj;
	gui_event_st status = GUIE_LEAVE;
	pstart();
	
	switch ( event->type )
	{
		case SDL_MOUSEMOTION:
		{
			obj = gui_search_by_coord(event->motion.x, event->motion.y);
			if ( obj != obj_mon )
			{
				// сменился фокус мышки
				if ( obj_mon )
				{
					if ( obj_mon->moff(obj_mon, 0, 0, 0) == GUIE_CRITICAL )
						return GUIE_CRITICAL;
				}
				obj_mon = obj;
			}
			if ( obj )
			{
				if ( obj->mon(obj, event->motion.x-obj->rx, \
					event->motion.y-obj->ry, 0) == GUIE_CRITICAL )
					return GUIE_CRITICAL;
				status = GUIE_EAT;
			}
			break;
		}
	}
	
	pstop();
	return status;
}
