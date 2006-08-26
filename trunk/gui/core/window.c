/***************************************************************************
 *            window.c
 *
 *  Sat Jun  3 17:15:07 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <libintl.h>

#include "window.h"
#include "ku2/errors/debug.h"
#include "ku2/other/other.h"
#include "gfx/gfx.h"

ecode_t gui_window_create( gui_obj_t *obj )
{
	gui_window_t *const arg=obj->arg;
	pstart();
	
	arg->img_name="";
	
	pstop();
	return E_NONE;
}

ecode_t gui_window_set( gui_obj_t *obj, gui_field_t field, void *data )
{
	gui_window_t *const arg=obj->arg;
	pstart();
	
	switch ( field )
	{
		case GUI_IMAGE:
		{
			arg->img_name=(char*)data;
			break;
		}
		default:
		{
			SET_ERROR(E_INVALID,gettext("Cannot set the attribute: invalid field."));
			return E_INVALID;
		}
	}
	
	pstop();
	return E_NONE;
}

ecode_t gui_window_get( gui_obj_t *obj, gui_field_t field, void *data )
{
	gui_window_t *const arg=obj->arg;
	pstart();
	
	switch ( field )
	{
		case GUI_IMAGE:
		{
			*(char**)data=arg->img_name;
			break;
		}
		default:
		{
			SET_ERROR(E_INVALID,gettext("Cannot get the attribute: invalid field."));
			return E_INVALID;
		}
	}
	
	pstop();
	return E_NONE;
}

ecode_t gui_window_load( gui_obj_t *obj )
{
	gui_window_t *const arg=obj->arg;
	pstart();
	
	arg->img=res_access(arg->img_name);
	if ( arg->img==NULL )
	{
		SET_ERROR(E_KU2,vstr(gettext("Cannot load a resource (kucode=%d)"),kucode));
		return E_KU2;
	}
	
	pstop();
	return E_NONE;
}

ecode_t gui_window_uload( gui_obj_t *obj )
{
	gui_window_t *const arg=obj->arg;
	pstart();
	
	if ( (kucode=res_release(arg->img_name))!=KE_NONE )
	{
		SET_ERROR(E_KU2,vstr(gettext("Cannot unload a resource (kucode=%d)"),kucode));
		return E_KU2;
	}
	
	pstop();
	return E_NONE;
}

ecode_t gui_window_draw( gui_obj_t *obj, int x, int y, int w, int h )
{
	gui_window_t *const arg=obj->arg;
	pstart();
	
	if ( gfx_draw(arg->img,screen,GFX_IMG_REAL,obj->rx,obj->ry,x,y,w,h)!=E_NONE )
		return ecode;
	
	pstop();
	return E_NONE;
}
