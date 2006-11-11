/***************************************************************************
 *            gui.c
 *
 *  Fri Jun  2 18:26:00 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <stdarg.h>

#include "gui.h"
#include "ku2/debug.h"
#include "ku2/ecode.h"
#include "ku2/memory.h"
#include "ku2/types.h"
#include "ds/list/list.h"
#include "ds/abtree/abtree.h"
#include "io/log/log.h"
#include "ku2/gettext.h"
#include "other/other.h"

//! Comparing function.
static int gui_compf( void *obj1, void *obj2 )
{
	return (int*)obj1-(int*)obj2;
}

//! Freeing function.
static int gui_freef( void *obj )
{
	((gui_obj_t*)obj)->destroyf(obj);
	if ( ((gui_obj_t*)obj)->children )
		dl_list_free(((gui_obj_t*)obj)->children, NULL);
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

//! All created objects.
static tree_t *gui_objects;

//! Last object ID.
static uint gui_lastid = 0;

static gui_obj_t
	*obj_root,		//!< Root object.
	*obj_mon,		//!< Object, where is the mouse.
	*obj_mdown;		//!< Object, where the mouse button was pressed.

kucode_t gui_init( void )
{
	pstart();

	gui_objects = abtree_create(gui_compf, 0);
	if ( gui_objects == NULL )
		return kucode;

	obj_root = NULL;
	obj_mon = NULL;
	obj_mdown = NULL;
	
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
		obj->destroyf(obj);
		dfree(obj);
		return NULL;
	}

	obj->id = gui_lastid++;

	pdebug("Object '%s` %u is created!\n", (char*)obj->widget, obj->id);
	pstop();
	return obj;
}

kucode_t gui_obj_delete( gui_obj_t *obj )
{
	static int recursion_lvl = 0;
	pstart();
	
	recursion_lvl++;
	
	// выгружаем подобъект
	if ( obj->status > GUI_NOTLOADED )
		obj->uload(obj);
	
	// удаляем подобъект
	obj->destroyf(obj);
	
	// удаляем потомков
	if ( obj->children )
	{
		if ( dl_list_first(obj->children) == KE_NONE )
			do gui_obj_delete(dl_list_cur(obj->children));
				while ( !dl_list_offside(obj->children) );
		dl_list_free(obj->children, NULL);
	}
	
	// исключаем из потомков предка
	if ( (recursion_lvl == 1) && obj->parent )
	{
		gui_excl_from_parent(obj);
	}

	pdebug("Object '%s` %u is deleted!\n", (char*)obj->widget, obj->id);
	dfree(obj);
	recursion_lvl--;
	
	pstop();
	return KE_NONE;
}

void gui_root( gui_obj_t *obj )
{
	pstart();
	
	obj_root = obj;
	
	pdebug("Object '%s` %u became a root!\n", (char*)obj->widget, obj->id);
	pstop();
}

kucode_t gui_move( gui_obj_t *obj, int x, int y )
{
	pstart();
	
	obj->x = x;
	obj->y = y;

	pdebug("Object '%s` %u was moved!\n", (char*)obj->widget, obj->id);
	pstop();
	return KE_NONE;
}

kucode_t gui_resize( gui_obj_t *obj, int w, int h )
{
	pstart();
	
	obj->width = w;
	obj->height = h;

	if ( obj->dim && (obj->dim(obj) != KE_NONE) )
		return kucode;
	
	pdebug("Object '%s` %u was resized!\n", (char*)obj->widget, obj->id);
	pstop();
	return KE_NONE;
}

kucode_t gui_ch_host( gui_obj_t *obj, gui_obj_t *host )
{
	pstart();
	
	ku_avoid( obj->parent == host );
	
	if ( host )
		if ( gui_ins_to_children(host, obj) != KE_NONE )
			return kucode;
	
	if ( obj->parent )
		gui_excl_from_parent(obj);
	obj->parent = host;
	
	pdebug("Object '%s` %u changed host!\n", (char*)obj->widget, obj->id);
	pstop();
	return KE_NONE;
}

kucode_t gui_set( gui_obj_t *obj, int parcnt, ... )
{
	va_list ap;
	int i, param;
	void *data;
	pstart();
	
	ku_avoid( parcnt <= 0 );
	
	va_start(ap, parcnt);
	
	if ( obj->set )
	{
		for ( i = 0; i < parcnt; i++ )
		{
			param = va_arg(ap, int);
			data = va_arg(ap, void*);
			if ( obj->set(obj, param, data) != KE_NONE )
			{
				va_end(ap);
				return kucode;
			}
		}
	}	else
	{
		va_end(ap);
		KU_ERRQ(KE_INVALID);
	}
	
	va_end(ap);
	pstop();
	return KE_NONE;
}

kucode_t gui_get( gui_obj_t *obj, int parcnt, ... )
{
	va_list ap;
	int i, param;
	void *data;
	pstart();
	
	ku_avoid( parcnt <= 0 );
	
	va_start(ap, parcnt);
	
	if ( obj->set )
	{
		for ( i = 0; i < parcnt; i++ )
		{
			param = va_arg(ap, int);
			data = va_arg(ap, void*);
			if ( obj->get(obj, param, data) != KE_NONE )
			{
				va_end(ap);
				return kucode;
			}
		}
	}	else
	{
		va_end(ap);
		KU_ERRQ(KE_INVALID);
	}
	
	va_end(ap);
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
			if ( obj->uload(obj) != KE_NONE )
				return kucode;
			break;
		}
		case GUI_ENABLED:
		{
			// загрузить объект, если он не загружен
			// сделать его доступным
			if ( obj->status == GUI_NOTLOADED )
			{
				if ( obj->load(obj) != KE_NONE )
					return kucode;
			}
			if ( obj->enable && (obj->enable(obj) != KE_NONE) )
				return kucode;
			break;
		}
		case GUI_DISABLED:
		{
			// сделать объект недоступным, но видимым
			if ( obj->disable && (obj->disable(obj) != KE_NONE) )
				return kucode;
			break;
		}
		case GUI_HIDDEN:
		{
			// скрыть объект
			if ( obj->hide && (obj->hide(obj) != KE_NONE) )
				return kucode;
		}
	}

	obj->status = status;

	pstop();
	return KE_NONE;
}

static kucode_t gui_obj_draw( gui_obj_t *obj, int x, int y )
{
	pstart();

	if ( obj->draw(obj, obj->x+x, obj->y+y) != KE_NONE )
		return kucode;

	if ( obj->children )
	{
		if ( dl_list_first(obj->children) == KE_NONE )
		{
			do
			{
				if ( gui_obj_draw(dl_list_next(obj->children), obj->x+x, obj->y+y) != KE_NONE )
					return kucode;
			}	while ( !dl_list_offside(obj->children) );
		}
	}

	pstop();
	return KE_NONE;
}

kucode_t gui_draw( void )
{
	return gui_obj_draw(obj_root, 0, 0);
}

static gui_obj_t *gui_search_by_coord( int x, int y )
{
	/*gui_obj_t *obj = obj_root;
	
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
			if  ( dl_list_offside(obj->children) )
				return obj;
		}
	}*/
	
	//return obj;
	return NULL;
}

gui_event_st gui_process( SDL_Event *event )
{
	//gui_obj_t *obj;
	gui_event_st status[2] = { GUIE_LEAVE, GUIE_LEAVE };
	/*pstart();
	
	switch ( event->type )
	{
		case SDL_MOUSEMOTION:
		{
			obj = gui_search_by_coord(event->motion.x, event->motion.y);
			if ( obj != obj_mon )
			{
				// сменился фокус мышки
				if ( obj_mon && obj_mon->moff )
					status[0] = obj_mon->moff(obj_mon, 0, 0, 0);
				obj_mon = obj;
			}
			if ( obj && obj->mon )
				status[1] = obj->mon(obj, event->motion.x-obj->rx, \
					event->motion.y-obj->ry, 0);
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			ku_avoid( obj_mdown != NULL );
			obj_mdown = gui_search_by_coord(event->button.x, event->button.y);
			if ( obj_mdown && obj_mdown->mdown )
				status[0] = obj_mdown->mdown(obj_mdown, event->button.x-obj_mdown->rx, \
					event->button.y-obj_mdown->ry, event->button.button);
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			if ( obj_mdown )
			{
				if ( obj_mdown->mup )
					status[0] = obj_mdown->mup(obj_mdown, event->button.x-obj_mdown->rx, \
						event->button.y-obj_mdown->ry, event->button.button);
				obj_mdown = NULL;
			}
			break;
		}
	}*/
	
	pstop();
	return MINint(status[0], status[1]);
}
