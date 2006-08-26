/***************************************************************************
 *            gui.c
 *
 *  Fri Jun  2 18:26:00 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <libintl.h>
#include <string.h>

#include "gui_core.h"
#include "window.h"
#include "button.h"

#include "ku2/list/list.template.c"
#include "ku2/list/list_end.h"

#include "ku2/other/other.h"
#include "ku2/log/log.h"

#define ABTREE_TYPE gui_obj_t*
#define ABTREE_EQ(a, b) (a->id==b->id)
#define ABTREE_MORE(a, b) (a->id>b->id)
#define ABTREE_ERRVAL NULL
#define ABTREE_ID gui
#include "ku2/abtree/abtree_core.h"
#include "ku2/abtree/abtree.template.c"
#include "ku2/abtree/abtree_end.h"

static abtree_t_gui *objects;	// список объектов
static gui_obj_t *root;			// корневой объект
static gui_obj_t *active;		// активный (текущий) объект
static gui_obj_t *clicked;		// для события CLICK
static gui_obj_t *motion;		// для события MOTION

static ecode_t gui_obj_rem( gui_obj_t *obj )
{	
	if ( obj->child!=NULL )
	{
		kucode=kulist_free_gui(obj->child);
		if ( kucode!=E_NONE )
		{
			SET_ERROR(E_KU2,vstr(gettext("Cannot free a list (kucode=%d)"),kucode));
			return E_KU2;
		}
	}
	
	if ( obj->loaded && (obj->arg_uload!=NULL) )
	{
		if ( obj->arg_uload(obj)!=E_NONE )
			return ecode;
	}
	
	if ( obj->arg_delete!=NULL )
	{
		if ( obj->arg_delete(obj)!=E_NONE )
			return ecode;
	}
	
	dfree(obj);
	return E_NONE;
}

static gui_obj_t *gui_obj_search( gui_id obj )
{
	gui_obj_t *pobj;
	gui_obj_t searchobj;
	static gui_obj_t *last_obj;
	static gui_id last_id=0;
	
	if ( obj==last_id )
		return last_obj;
	
	searchobj.id=obj;
	pobj=abtree_search_gui(objects,&searchobj);
	if ( pobj==NULL )
		SET_ERROR(E_KU2,vstr(gettext("Cannot find an object width id %d (kucode=%d)"),obj,kucode));
	
	last_obj=pobj;
	last_id=obj;
	
	return pobj;
}

static gui_id gui_free_id( void )
{
	static gui_id id=0;
	id++;
	return id;
}

static void gui_realcoord( gui_obj_t *obj )
{
	gui_obj_t *tobj=obj->parent;
	obj->rx=obj->x;
	obj->ry=obj->y;
	while ( tobj!=NULL )
	{
		obj->rx+=tobj->x;
		obj->ry+=tobj->y;
		tobj=tobj->parent;
	}
}

ecode_t gui_init( void )
{
	pstart();
	
	objects=abtree_create_gui();
	if ( objects==NULL )
	{
		SET_ERROR(E_KU2,vstr(gettext("Failed to initialize a GUI, cannot create a tree: %d"),kucode));
		return E_KU2;
	}
	
	root=NULL;
	active=NULL;
	clicked=NULL;
	motion=NULL;
	
	pstop();
	return E_NONE;
}

ecode_t gui_halt( void )
{
	gui_obj_t *obj;
	pstart();
	
	abtree_goto_first_gui(objects);
	while ( (obj=abtree_goto_next_gui(objects))!=NULL )
	{
		if ( gui_obj_rem(obj)!=E_NONE )
		{
			plog(vstr(gettext("Error in '%s`: %s (code=%d)"),efunc,etext,ecode));
		}
	}
	
	kucode=abtree_free_gui(objects);
	if ( kucode!=KE_NONE ) return E_KU2;
	
	pstop();
	return E_NONE;
}

gui_id gui_obj_create( gui_type_t type )
{
	gui_obj_t *obj;
	size_t obj_sz;
	pstart();
	
	switch ( type )
	{
		case GUI_WINDOW:
		{
			obj_sz=sizeof(gui_window_t);
			break;
		}
		case GUI_BUTTON:
		{
			obj_sz=sizeof(gui_button_t);
			break;
		}
		default:
		{
			SET_ERROR(E_INVALID,vstr(gettext("Invalid GUI object type: %s"),type));
			return 0;
		}
	}
	
	obj=dmalloc(sizeof(gui_obj_t)+obj_sz);
	if ( obj==NULL )
	{
		SET_ERROR_MEMORY();
		return E_MEMORY;
	}
	
	switch ( type )
	{
		case GUI_WINDOW:
		{
			obj->arg_create=gui_window_create;
			obj->arg_clone=NULL;
			obj->arg_delete=NULL;
			obj->arg_set=gui_window_set;
			obj->arg_get=gui_window_get;
			obj->arg_load=gui_window_load;
			obj->arg_uload=gui_window_uload;
			obj->arg_draw=gui_window_draw;
			obj->mouse_on=NULL;
			obj->mouse_off=NULL;
			obj->mouse_down=NULL;
			obj->mouse_up=NULL;
			obj->key_down=NULL;
			break;
		}
		case GUI_BUTTON:
		{
			obj->arg_create=gui_button_create;
			obj->arg_clone=NULL;
			obj->arg_delete=NULL;
			obj->arg_set=gui_button_set;
			obj->arg_get=gui_button_get;
			obj->arg_load=gui_button_load;
			obj->arg_uload=gui_button_uload;
			obj->arg_draw=gui_button_draw;
			obj->mouse_on=gui_button_mouseon;
			obj->mouse_off=gui_button_mouseoff;
			obj->mouse_down=gui_button_mousedown;
			obj->mouse_up=gui_button_mouseup;
			obj->key_down=NULL;
			break;
		}
		default: {}
	}
	
	obj->arg=(char*)obj+sizeof(gui_obj_t);
	if ( obj->arg_create!=NULL )
		if ( obj->arg_create(obj)!=E_NONE )
		{
			dfree(obj);
			return 0;
		}
	
	obj->id=gui_free_id();
	obj->parent=NULL;
	obj->type=type;
	obj->child=NULL;
	obj->x=obj->y=obj->width=obj->height=0;
	obj->enabled=1;
	obj->loaded=0;
	obj->clicked=NULL;
	obj->keypressed=NULL;
	
	kucode=abtree_ins_gui(objects,obj);
	if ( kucode!=KE_NONE )
	{
		SET_ERROR(E_KU2,vstr(gettext("Failed to add a GUI object to the objects' list (kucode=%d)"),kucode));
		dfree(obj);
		return 0;
	}
	
	pstop();
	return obj->id;
}

gui_id gui_obj_clone( gui_id obj )
{
	gui_obj_t *pobj, *nobj;
	size_t obj_sz;
	pstart();
	
	pobj=gui_obj_search(obj);
	if ( pobj==NULL )
		return ecode;
	
	switch ( pobj->type )
	{
		case GUI_WINDOW:
		{
			obj_sz=sizeof(gui_window_t);
			break;
		}
		case GUI_BUTTON:
		{
			obj_sz=sizeof(gui_button_t);
			break;
		}
		default:
		{
			SET_ERROR(E_INVALID,vstr(gettext("Invalid GUI object type: %s"),pobj->type));
			return 0;
		}
	}
	
	nobj=dmalloc(sizeof(gui_obj_t)+obj_sz);
	if ( nobj==NULL )
	{
		SET_ERROR_MEMORY();
		return 0;
	}
	memcpy(nobj,pobj,sizeof(gui_obj_t)+obj_sz);
	
	nobj->arg=(char*)nobj+sizeof(gui_obj_t);
	if ( nobj->arg_clone!=NULL )
		if ( nobj->arg_clone(nobj)!=E_NONE )
		{
			dfree(nobj);
			return 0;
		}
	
	nobj->id=gui_free_id();
	nobj->child=NULL;
	nobj->loaded=0;
	
	pstop();
	return nobj->id;
}

ecode_t gui_obj_delete( gui_id obj )
{
	gui_obj_t *pobj;
	pstart();
	
	pobj=gui_obj_search(obj);
	if ( pobj==NULL )
		return ecode;
	
	kucode=abtree_rem_gui(objects,pobj);
	if ( kucode!=KE_NONE )
	{
		SET_ERROR(E_KU2,vstr(gettext("Failed to remove a GUI object from the objects' list (kucode=%d)"),kucode));
		return E_KU2;
	}
	
	if ( gui_obj_rem(pobj)!=E_NONE )
	{
		plog(vstr(gettext("Failed to delete a GUI subobject: %s (code=%d)"),etext,ecode));
	}
	
	pstop();
	return E_NONE;
}

ecode_t gui_obj_set( gui_id obj, gui_field_t field, void *data )
{
	gui_obj_t *pobj;
	pstart();
	
	pobj=gui_obj_search(obj);
	if ( pobj==NULL )
		return ecode;
	
	if ( pobj->loaded )
	{
		SET_ERROR(E_ORDER,gettext("Cannot set the attribute: object is loaded."));
		return E_ORDER;
	}
	
	switch ( field )
	{
		case GUI_X:
		{
			pobj->x=(int)data;
			break;
		}
		case GUI_Y:
		{
			pobj->y=(int)data;
			break;
		}
		case GUI_WIDTH:
		{
			pobj->width=(int)data;
			break;
		}
		case GUI_HEIGHT:
		{
			pobj->height=(int)data;
			break;
		}
		case GUI_ENABLED:
		{
			pobj->enabled=(int)data;
			break;
		}
		case GUI_CB_CLICK:
		{
			pobj->clicked=(gui_cb_f)data;
			break;
		}
		default:
		{
			if ( pobj->arg_set!=NULL )
			{
				if ( pobj->arg_set(pobj,field,data)!=E_NONE )
					return ecode;
			}	else
			{
				SET_ERROR(E_INVALID,gettext("Cannot set the attribute: invalid field."));
				return E_INVALID;
			}
		}
	}
	
	pstop();
	return E_NONE;
}

ecode_t gui_obj_get( gui_id obj, gui_field_t field, void *data )
{
	gui_obj_t *pobj;
	pstart();
	
	pobj=gui_obj_search(obj);
	if ( pobj==NULL )
		return ecode;
	
	switch ( field )
	{
		case GUI_X:
		{
			*(int*)data=pobj->x;
			break;
		}
		case GUI_Y:
		{
			*(int*)data=pobj->y;
			break;
		}
		case GUI_WIDTH:
		{
			*(int*)data=pobj->width;
			break;
		}
		case GUI_HEIGHT:
		{
			*(int*)data=pobj->height;
			break;
		}
		case GUI_ENABLED:
		{
			*(int*)data=pobj->enabled;
			break;
		}
		default:
		{
			if ( pobj->arg_get!=NULL )
			{
				if ( pobj->arg_get(pobj,field,data)!=E_NONE )
					return ecode;
			}	else
			{
				SET_ERROR(E_INVALID,gettext("Cannot get the attribute: invalid field."));
				return E_INVALID;
			}
		}
	}
	
	pstop();
	return E_NONE;
}

ecode_t gui_obj_load( gui_id obj )
{
	gui_obj_t *pobj;
	pstart();
	
	pobj=gui_obj_search(obj);
	if ( pobj==NULL )
		return ecode;
	
	if ( pobj->loaded )
	{
		SET_ERROR(E_ORDER,gettext("Cannot load an object: object is loaded."));
		return E_ORDER;
	}
	
	if ( pobj->arg_load!=NULL )
	{
		if ( pobj->arg_load(pobj)!=E_NONE )
			return ecode;
	}
	
	pobj->loaded=1;
	
	pstop();
	return E_NONE;
}

ecode_t gui_obj_uload( gui_id obj )
{
	gui_obj_t *pobj;
	pstart();
	
	pobj=gui_obj_search(obj);
	if ( pobj==NULL )
		return ecode;
	
	if ( !pobj->loaded )
	{
		SET_ERROR(E_ORDER,gettext("Cannot unload an object: object is not loaded."));
		return E_ORDER;
	}
	
	if ( pobj->arg_uload!=NULL )
	{
		if ( pobj->arg_uload(pobj)!=E_NONE )
			return ecode;
	}
	
	pobj->loaded=0;
	
	pstop();
	return E_NONE;
}

ecode_t gui_obj_attach( gui_id obj, gui_id dest, int x, int y )
{
	gui_obj_t *pobj, *pdest;
	pstart();
	
	pobj=gui_obj_search(obj);
	if ( pobj==NULL )
		return ecode;
	
	pdest=gui_obj_search(dest);
	if ( pdest==NULL )
		return ecode;
	
	if ( pobj->parent!=NULL )
		if ( gui_obj_release(obj)!=E_NONE )
			return ecode;
	
	if ( pdest->child==NULL )
	{
		//	создаём список "детей"
		pdest->child=kulist_create_gui();
		if ( pdest->child==NULL )
		{
			SET_ERROR(E_KU2,vstr(gettext("Failed to create a children list (kucode=%d)"),kucode));
			return E_KU2;
		}
	}
	
	kucode=kulist_ins_last_gui(pdest->child,pobj);
	if ( kucode!=KE_NONE )
	{
		SET_ERROR(E_KU2,vstr(gettext("Failed to add a child to the list (kucode=%d)"),kucode));
		if ( pdest->child->size==0 )
		{
			kulist_free_gui(pdest->child);
			pdest->child=NULL;
		}
		return E_KU2;
	}
	
	pobj->parent=pdest;
	pobj->x=x;
	pobj->y=y;
	gui_realcoord(pobj);
	
	pstop();
	return E_NONE;
}

ecode_t gui_obj_release( gui_id obj )
{
	gui_obj_t *pobj, *parent;
	pstart();
	
	pobj=gui_obj_search(obj);
	if ( pobj==NULL )
		return ecode;
	parent=pobj->parent;
	
	if ( parent==NULL )
	{
		SET_ERROR(E_INVALID,gettext("Object is not attached."));
		return E_INVALID;
	}
	
	kucode=kulist_search_gui(pobj->child,pobj);
	if ( kucode!=KE_NONE )
	{
		SET_ERROR(E_KU2,vstr(gettext("Failed to find a child in list (kucode=%d)"),kucode));
		return E_KU2;
	}
	
	kucode=kulist_rem_gui(pobj->child);
	if ( kucode!=KE_NONE )
	{
		SET_ERROR(E_KU2,vstr(gettext("Failed to remove a child from list (kucode=%d)"),kucode));
		return E_KU2;
	}
	
	pobj->parent=NULL;
	
	if ( parent->child->size==0 )
	{
		kucode=kulist_free_gui(parent->child);
		SET_ERROR(E_KU2,vstr(gettext("Cannot delete a parent list (kucode=%d)"),kucode));
		return E_KU2;
	}
	
	pstop();
	return E_NONE;
}

ecode_t gui_set_root( gui_id obj )
{
	gui_obj_t *pobj;
	pstart();
	
	if ( obj==0 )
	{
		root=NULL;
	}	else
	{
		pobj=gui_obj_search(obj);
		if ( pobj==NULL )
			return ecode;
		root=pobj;
		gui_realcoord(pobj);
	}
	
	pstop();
	return E_NONE;
}

ecode_t gui_set_active( gui_id obj )
{
	gui_obj_t *pobj;
	pstart();
	
	if ( obj==0 )
	{
		active=NULL;
	}	else
	{
		pobj=gui_obj_search(obj);
		if ( pobj==NULL )
			return ecode;
		active=pobj;
	}
	
	pstop();
	return E_NONE;
}

ecode_t gui_draw( gui_obj_t *obj, int x, int y, int w, int h )
{
	gui_obj_t *child;
	int cx, cy, cw, ch;
	pstart();
	
	if ( !obj->loaded )
	{
		SET_ERROR(E_ORDER,gettext("Cannot draw an unloaded object."));
		return E_ORDER;
	}
	
	//	рисование объекта
	if ( obj->arg_draw(obj,x,y,w,h)!=E_NONE )
		return ecode;
	
	//	рисование подобъектов
	if ( obj->child!=NULL )
	{
		child=kulist_first_gui(obj->child);
		if ( child==NULL )
		{
			SET_ERROR(E_KU2,vstr(gettext("Cannot get the first element of the list (kucode=%d)"),kucode));
			return E_KU2;
		}
		do
		{
			if ( (child->x+child->width>x) && (child->x<x+w) &&
				(child->y+child->height>y) && (child->y<y+h) )
			{
				if ( (cx=x-child->x)<0 ) cx=0;
				if ( (cy=y-child->y)<0 ) cy=0;
				if ( w-child->x-cx<child->width ) cw=w-child->x-cx; else cw=child->width;
				if ( h-child->y-cy<child->height ) ch=h-child->y-cy; else ch=child->height;
				if ( gui_draw(child,cx,cy,cw,ch)!=E_NONE )
					return ecode;
			}
			child=kulist_next_gui(obj->child);
		}	while ( !kulist_isfirst_gui(obj->child) );
	}
	/*
	//	рисование объектов, которые покравают текущий объект
	if ( obj->parent!=NULL )
	{
		kucode=kulist_search_gui(obj->parent->child,obj);
		if ( kucode!=KE_NONE )
		{
			SET_ERROR(E_KU2,vstr(gettext("Failed to find a child in list (kucode=%d)"),kucode));
			return E_KU2;
		}
		obj=obj->parent;
		do
		{
			child=kulist_next_gui(obj->child);
			if ( child==NULL )
			{
				SET_ERROR(E_KU2,vstr(gettext("Cannot get the next element of the list (kucode=%d)"),kucode));
				return E_KU2;
			}
			//	SJUDA RISOVANIJE
		}	while ( !kulist_islast_gui(obj->child) );
	}
	*/
	pstop();
	return E_NONE;
}

ecode_t gui_redraw( void )
{
	return gui_draw(root,0,0,root->width,root->height);
}

ecode_t gui_update( gui_obj_t *obj, int x, int y, int w, int h )
{
	const gui_obj_t *const parent=obj->parent;
	int cx, cy, cw, ch;
	if ( obj->x+x<0 ) cx=-obj->x; else cx=x;
	if ( obj->y+y<0 ) cy=-obj->y; else cy=y;
	if ( w+obj->x>parent->width ) cw=parent->width-cx-obj->x; else 
	if ( h+obj->y>parent->height ) ch=parent->height-cy-obj->y;
}

static gui_obj_t *gui_obj_coord( int x, int y, int *rx, int *ry )
{
	gui_obj_t *obj, *pobj;
	int found;
	
	if ( root && (root->enabled) && (root->x<=x) && (root->x+root->width>=x) &&
		(root->y<=y) && (root->y+root->height>=y) )
	{
		*rx=root->x;
		*ry=root->y;
		obj=root;
		do
		{
			found=0;
			if ( obj->child!=NULL )
			{
				pobj=kulist_first_gui(obj->child);
				if ( pobj==NULL )
				{
					plog(gettext("Object %d child list is empty (kucode=%d)"),obj->id,kucode);
					return root;
				}
				do
				{
					if ( (pobj->enabled) && (*rx+pobj->x<=x) && (*rx+pobj->x+pobj->width>=x) &&
						(*ry+pobj->y<=y) && (*ry+pobj->y+pobj->height>=y) )
					{
						*rx+=obj->x;
						*ry+=obj->y;
						obj=pobj;
						found=1;
						break;
					}
					pobj=kulist_next_gui(obj->child);
				}	while ( !kulist_isfirst_gui(obj->child) );
			}
		}	while ( found );
		return obj;
	}	else
	{
		return NULL;
	}
}

ecode_t gui_manage_event( SDL_Event *event )
{
	gui_obj_t *obj;
	int x=0, y=0;
	
	switch ( event->type )
	{
		case SDL_MOUSEMOTION:
		{
			obj=gui_obj_coord(event->motion.x,event->motion.y,&x,&y);
			if ( motion!=obj )
			{
				if ( (motion!=NULL) && (motion->mouse_off!=NULL) )
					if ( motion->mouse_off(motion,event,x,y)!=E_NONE )
						return ecode;
				motion=obj;
			}
			if ( (obj!=NULL) && (obj->mouse_on!=NULL) )
					if ( obj->mouse_on(obj,event,x,y)!=E_NONE )
						return ecode;
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			obj=gui_obj_coord(event->button.x,event->button.y,&x,&y);
			if ( obj!=NULL )
			{
				if ( obj->mouse_down!=NULL )
					if ( obj->mouse_down(obj,event,x,y)!=E_NONE )
						return ecode;
				clicked=obj;
				active=obj;
			}
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			if ( clicked!=NULL )
			{
				if ( clicked->mouse_up!=NULL )
					if ( clicked->mouse_up(clicked,event,x,y)!=E_NONE )
						return ecode;
				if ( clicked->clicked!=NULL )
					if ( clicked->clicked(clicked->id,NULL)!=E_NONE )
						return ecode;
				clicked=NULL;
			}
			break;
		}
		case SDL_KEYDOWN:
		{
			if ( active!=NULL )
			{
				if ( active->key_down!=NULL )
					if ( active->key_down(active,event,x,y)!=E_NONE )
						return ecode;
				if ( active->keypressed!=NULL )
					if ( active->keypressed(active->id,(void*)event->key.keysym.sym)!=E_NONE )
						return ecode;
			}
			break;
		}
	}
	
	return E_NONE;
}
