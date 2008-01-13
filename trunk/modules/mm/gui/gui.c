/*
		gui.c
		Tue Dec 25 16:04:24 2007

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2007
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

#include <stdarg.h>

#if !defined(NO_NVIDIA_GL_H)
#include <nvidia/GL/gl.h>
#else
#include <GL/gl.h>
#endif

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
static int gui_compf( const void *obj1, const void *obj2 )
{
	return (int*)obj1-(int*)obj2;
}

//! Freeing function.
static int gui_freef( void *obj )
{
	if ( ((gui_obj_t*)obj)->destroyf )
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
			return KU_GET_ERROR();
	}
	if ( dl_list_ins_last(parent->children, child) != KE_NONE )
		return KU_GET_ERROR();

	return KE_NONE;
}

//! All created objects.
static tree_t *gui_objects;

//! Last object ID.
static uint gui_lastid = 0;

static uint gui_w,	//!< Screen width.
	gui_h;			//!< Screen height.

static gui_obj_t
	*obj_root,		//!< Root object.
	*obj_mon,		//!< Object, where the mouse is.
	*obj_mdown,		//!< Object, where the mouse button was pressed.
	*obj_focus;		//!< Object, where the keyboard focus is.

static int gui_lrx,	//!< Real X of the found object.
	gui_lry,		//!< Real Y of the found object.
	gui_mdown_rx,	//!< Real X of object, where the mouse button was pressed.
	gui_mdown_ry;	//!< Real Y of object, where the mouse button was pressed.

static int gui_redraw = 0;

kucode_t gui_init( uint w, uint h )
{
	pstartp("w = %u, h = %u", w, h);
	
	gui_objects = abtree_create(gui_compf, 0);
	if ( gui_objects == NULL )
		KU_ERRQ_PASS();
	
	gui_w = w;
	gui_h = h;
	
	obj_root = NULL;
	obj_mon = NULL;
	obj_mdown = NULL;
	obj_focus = NULL;
	
	preturn KE_NONE;
}

kucode_t gui_halt( void )
{
	pstart();
	
	KU_WITHOUT_ERROR(abtree_free(gui_objects, gui_freef));
	
	preturn KE_NONE;
}

gui_obj_t *gui_obj_create( gui_load_f initf, uint widget_sz, ku_flag32_t flags )
{
	gui_obj_t *obj;
	pstart();
	
	obj = dmalloc(sizeof(gui_obj_t)+widget_sz);
	if ( obj == NULL )
		KU_ERRQ_VALUE(KE_MEMORY, NULL);
	
	obj->parent = NULL;
	obj->children = NULL;
	obj->status = GUI_NOTLOADED;
	obj->flags = flags;
	obj->updated = 0;
	obj->widget_sz = widget_sz;
	
	obj->initf = initf;
	obj->destroyf = NULL;
	
	obj->load = \
	obj->uload = \
	obj->enable = \
	obj->disable = \
	obj->hide = \
	obj->update = NULL;
	
	obj->dim = NULL;
	
	obj->set = \
	obj->get = NULL;
	
	obj->mon = \
	obj->moff = \
	obj->mdown = \
	obj->mup = NULL;
	
	obj->kdown = \
	obj->kup = NULL;
	
	obj->defocus = NULL;
	
	obj->draw = NULL;
	
	obj->widget = (int8_t*)obj+sizeof(gui_obj_t);
	
	if ( initf(obj) != KE_NONE )
	{
		dfree(obj);
		preturn NULL;
	}
	
	if ( abtree_ins(gui_objects, obj) != KE_NONE )
	{
		if ( obj->destroyf )
			obj->destroyf(obj);
		dfree(obj);
		preturn NULL;
	}
	
	obj->id = gui_lastid++;
	
	#ifdef GUI_LOG_ACTIVITIES
	plogfn_i("GUI", "Object '%s` %u is created!\n", (char*)obj->widget, obj->id);
	#endif
	preturn obj;
}

kucode_t gui_obj_delete( gui_obj_t *obj )
{
	static int recursion_lvl = 0;
	ku_avoid( obj == NULL );
	pstartp("obj = %p, obj->id = %u", obj, obj->id);
	
	recursion_lvl++;
	
	// выгружаем подобъект
	//gui_ch_status(obj, GUI_NOTLOADED);
	
	// удаляем подобъект
	if ( obj->destroyf )
		obj->destroyf(obj);
	
	// удаляем потомков
	if ( obj->children )
	{
		if ( dl_list_first(obj->children) == KE_NONE )
			do gui_obj_delete(dl_list_next(obj->children));
				while ( !dl_list_offside(obj->children) );
		dl_list_free(obj->children, NULL);
	}
	
	// исключаем из потомков предка
	if ( (recursion_lvl == 1) && obj->parent )
	{
		gui_excl_from_parent(obj);
	}
	
	// удаляем из списка объектов
	abtree_rem(gui_objects, obj, NULL);
	
	#ifdef GUI_LOG_ACTIVITIES
	plogfn_i("GUI", "Object '%s` %u is deleted!\n", (char*)obj->widget, obj->id);
	#endif
	
	dfree(obj);
	recursion_lvl--;
	
	preturn KE_NONE;
}

void gui_root( gui_obj_t *obj )
{
	pstartp("obj = %p, obj->id = %u", obj, obj ? obj->id : -1);
	
	obj_root = obj;
	obj_mon = NULL;
	obj_mdown = NULL;
	obj_focus = NULL;
	
	#ifdef GUI_LOG_ACTIVITIES
	plogfn_i("GUI", "Object '%s` %u became a root!\n", obj ? (char*)obj->widget : "NULL", obj ? obj->id : (uint)-1);
	#endif
	pstop();
}

kucode_t gui_move( gui_obj_t *obj, int x, int y )
{
	ku_avoid( obj == NULL );
	pstartp("obj = %p, obj->id = %u", obj, obj->id);
	
	obj->x = x;
	obj->y = y;
	
	#ifdef GUI_LOG_ACTIVITIES
	plogfn_i("GUI", "Object '%s` %u was moved!\n", (char*)obj->widget, obj->id);
	#endif
	preturn KE_NONE;
}

kucode_t gui_resize( gui_obj_t *obj, int w, int h )
{
	pstartp("obj = %p, obj->id = %u", obj, obj->id);
	
	ku_avoid( obj == NULL );
	
	obj->width = w;
	obj->height = h;
	
	if ( obj->dim && (obj->dim(obj) != KE_NONE) )
		KU_ERRQ_PASS();
	
	#ifdef GUI_LOG_ACTIVITIES
	plogfn_i("GUI", "Object '%s` %u was resized!\n", (char*)obj->widget, obj->id);
	#endif
	preturn KE_NONE;
}

kucode_t gui_ch_host( gui_obj_t *obj, gui_obj_t *host )
{
	pstartp("obj = %p, obj->id = %u", obj, obj->id);
	
	ku_avoid( (obj == NULL) || (obj->parent == host) );
	
	if ( host )
		if ( gui_ins_to_children(host, obj) != KE_NONE )
			KU_ERRQ_PASS();
	
	if ( obj->parent )
		gui_excl_from_parent(obj);
	obj->parent = host;
	
	#ifdef GUI_LOG_ACTIVITIES
	plogfn_i("GUI", "Object '%s` %u changed host!\n", (char*)obj->widget, obj->id);
	#endif
	preturn KE_NONE;
}

kucode_t gui_focus( gui_obj_t *obj )
{
	pstartp("obj = %p, obj->id = %u", obj, obj->id);
	
	ku_avoid( (obj == NULL) || (obj == obj_focus) );
	
	if ( obj_focus && obj_focus->defocus && (obj_focus->defocus(obj_focus) == GUIE_CRITICAL) )
		KU_ERRQ_PASS();
	obj_focus = obj;
	
	#ifdef GUI_LOG_ACTIVITIES
	plogfn_i("GUI", "Object '%s` %u has become focused!\n", (char*)obj->widget, obj->id);
	#endif
	preturn KE_NONE;
}

kucode_t gui_set( gui_obj_t *obj, int parcnt, ... )
{
	va_list ap;
	int i, param;
	void *data;
	pstartp("obj = %p, obj->id = %u", obj, obj->id);
	
	ku_avoid( (obj == NULL) || (parcnt <= 0) );
	
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
				KU_ERRQ_PASS();
			}
		}
		if ( obj->update && (obj->status > GUI_NOTLOADED) )
			obj->update(obj);
	}	else
	{
		va_end(ap);
		KU_ERRQ(KE_INVALID);
	}
	
	gui_redraw_later();
	
	va_end(ap);
	preturn KE_NONE;
}

kucode_t gui_get( gui_obj_t *obj, int parcnt, ... )
{
	va_list ap;
	int i, param;
	void *data;
	pstartp("obj = %p, obj->id = %u", obj, obj->id);
	
	ku_avoid( (obj == NULL) || (parcnt <= 0) );
	
	va_start(ap, parcnt);
	
	if ( obj->get )
	{
		for ( i = 0; i < parcnt; i++ )
		{
			param = va_arg(ap, int);
			data = va_arg(ap, void*);
			if ( obj->get(obj, param, data) != KE_NONE )
			{
				va_end(ap);
				KU_ERRQ_PASS();
			}
		}
	}	else
	{
		va_end(ap);
		KU_ERRQ(KE_INVALID);
	}
	
	va_end(ap);
	preturn KE_NONE;
}

kucode_t gui_ch_status( gui_obj_t *obj, gui_status_t status )
{
	pstartp("obj = %p, obj->id = %u", obj, obj->id);
	
	ku_avoid( obj == NULL );
	
	if ( obj->status != status )
	switch ( status )
	{
		case GUI_NOTLOADED:
		{
			// выгрузить объект
			if ( obj->uload && (obj->uload(obj) != KE_NONE) )
				KU_ERRQ_PASS();
			break;
		}
		case GUI_ENABLED:
		{
			// загрузить объект, если он не загружен
			// сделать его доступным
			if ( obj->status == GUI_NOTLOADED )
			{
				if ( obj->load && (obj->load(obj) != KE_NONE) )
					KU_ERRQ_PASS();
			}
			if ( obj->enable && (obj->enable(obj) != KE_NONE) )
				KU_ERRQ_PASS();
			break;
		}
		case GUI_DISABLED:
		{
			// сделать объект недоступным, но видимым
			if ( obj->disable && (obj->disable(obj) != KE_NONE) )
				KU_ERRQ_PASS();
			break;
		}
		case GUI_HIDDEN:
		{
			// скрыть объект
			if ( obj->hide && (obj->hide(obj) != KE_NONE) )
				KU_ERRQ_PASS();
		}
	}
	_
	obj->status = status;
	_
	preturn KE_NONE;
}

static kucode_t gui_obj_draw( gui_obj_t *obj, int x, int y )
{
	pstartp("obj = %p, obj->id = %u", obj, obj->id);
	
	ku_avoid( obj == NULL );
	_
	pdebug("%d: obj->draw = %p\n", obj->id, obj->draw);
	if ( obj->draw(obj, obj->x+x, obj->y+y) != KE_NONE )
		KU_ERRQ_PASS();
	_
	if ( obj->children )
	{
		if ( dl_list_first(obj->children) == KE_NONE )
		{
			_
			do
			{
				_
				if ( gui_obj_draw(dl_list_next(obj->children), obj->x+x, obj->y+y) != KE_NONE )
					KU_ERRQ_PASS();
			}	while ( !dl_list_offside(obj->children) );
			_
		}
	}
	
	preturn KE_NONE;
}

kucode_t gui_draw( void )
{
	pstart();
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	/*
	glFrustum(GUI_PROJ_LEFT, -GUI_PROJ_LEFT, \
		GUI_PROJ_BOTTOM, -GUI_PROJ_BOTTOM, 0.1, 2);
	*/
	//glOrtho(-4, 4, -3, 3, 1, -1);
	//glMatrixMode(GL_MODELVIEW);

	if ( gui_redraw && obj_root )
	{
		gui_redraw = 0;
		preturn gui_obj_draw(obj_root, 0, 0);
	}
	
	preturn KE_NONE;
}

void gui_redraw_later( void )
{
	gui_redraw = 1;
}

static inline gui_obj_t *gui_search_by_coord( int x, int y )
{
	pstart();
	/*static gui_obj_t *last_obj = NULL;*/
	gui_obj_t *obj;
	
	/*if ( last_obj && !last_obj->children && \
		(last_obj->x <= x) && (last_obj->x+last_obj->width >= x) && \
		(last_obj->y <= y) && (last_obj->y+last_obj->height >= y) )
		return last_obj;*/
	
	obj = obj_root;
	if ( (!obj) || (obj->status == GUI_HIDDEN) || (obj->x > x) || \
		(obj->x+obj->width < x) || (obj->y > y) || (obj->y+obj->height < y ) )
		{ preturn NULL; }
	
	gui_lrx = obj->x;
	gui_lry = obj->y;
	while ( obj->children && (obj->children->size > 0) )
	{
		dl_list_last(obj->children);
		for (;;)
		{
			gui_obj_t *nobj = dl_list_prev(obj->children);
			if ( (nobj->status < GUI_HIDDEN) && \
				(nobj->x+gui_lrx <= x) && (nobj->x+gui_lrx+nobj->width >= x) && \
				(nobj->y+gui_lry <=y ) && (nobj->y+gui_lry+nobj->height >= y) )
			{
				obj = nobj;
				gui_lrx += obj->x;
				gui_lry += obj->y;
				break;
			}
			if  ( dl_list_offside(obj->children) )
				{ preturn /*last_obj = */obj; }
		}
	}
	
	preturn /*last_obj = */obj;
}

gui_event_st gui_process( SDL_Event *event )
{
	gui_obj_t *obj;
	gui_event_st status[2] = { GUIE_LEAVE, GUIE_LEAVE };
	pstart();
	
	switch ( event->type )
	{
		case SDL_MOUSEMOTION:
		{
			//event->motion.y = gui_h-event->motion.y;
			obj = gui_search_by_coord(event->motion.x, event->motion.y);
			if ( obj != obj_mon )
			{
				// сменился фокус мышки
				pdebug("Mouse focus: '%s` %u (%d,%d :: %d,%d)\n", obj->widget, obj->id, obj->x, obj->y, obj->width, obj->height);
				if ( obj_mon && obj_mon->moff )
					status[0] = obj_mon->moff(obj_mon, 0, 0, 0);
				obj_mon = obj;
			}
			if ( obj && (obj->status < GUI_DISABLED ) && obj->mon )
				status[1] = obj->mon(obj, event->motion.x-gui_lrx, \
					event->motion.y-gui_lry, 0);
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			_
			ku_avoid( obj_mdown != NULL );
			//event->button.y = gui_h-event->button.y;
			obj_mdown = gui_search_by_coord(event->button.x, event->button.y);
			if ( obj_mdown && (obj_mdown->status < GUI_DISABLED) && obj_mdown->mdown )
				status[0] = obj_mdown->mdown(obj_mdown, event->button.x-gui_lrx, \
					event->button.y-gui_lry, event->button.button);
			gui_mdown_rx = gui_lrx;
			gui_mdown_ry = gui_lry;
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			_
			if ( obj_mdown && (obj_mdown->status < GUI_DISABLED) )
			{
				if ( obj_mdown->mup )
				{
					//event->motion.y = gui_h-event->motion.y;
					status[0] = obj_mdown->mup(obj_mdown, event->button.x-gui_mdown_rx, \
						event->button.y-gui_mdown_ry, event->button.button);
				}
			}
			obj_mdown = NULL;
			break;
		}
		case SDL_KEYDOWN:
		{
			if ( obj_focus && (obj_focus->status < GUI_DISABLED) )
			{
				status[0] = obj_focus->kdown(obj_focus, event->key.keysym.sym);
			}
			break;
		}
	}
	
	preturn MINint(status[0], status[1]);
}
