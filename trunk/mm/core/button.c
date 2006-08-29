/***************************************************************************
 *            button.c
 *
 *  Sun Jun 18 17:18:41 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <libintl.h>

#include "button.h"
#include "ku2/errors/debug.h"
#include "ku2/other/other.h"
#include "gfx/gfx.h"

ecode_t gui_button_create( gui_obj_t *obj )
{
	gui_button_t *const arg=obj->arg;
	pstart();
	
	arg->img_name="";
	arg->img_mon_name="";
	arg->img_mdown_name="";
	arg->font_name="";
	arg->font_colour.r=0;
	arg->font_colour.g=0;
	arg->font_colour.b=0;
	arg->text_face=NULL;
	arg->text="";
	
	pstop();
	return E_NONE;
}

ecode_t gui_button_set( gui_obj_t *obj, gui_field_t field, void *data )
{
	gui_button_t *const arg=obj->arg;
	pstart();
	
	switch ( field )
	{
		case GUI_IMAGE:
		{
			arg->img_name=(char*)data;
			break;
		}
		case GUI_IMG_MOUSEON:
		{
			arg->img_mon_name=(char*)data;
			break;
		}
		case GUI_IMG_CLICK:
		{
			arg->img_mdown_name=(char*)data;
			break;
		}
		case GUI_FONT:
		{
			arg->font_name=(char*)data;
			break;
		}
		case GUI_FNT_COLOUR:
		{
			arg->font_colour=*(SDL_Color*)data;
			break;
		}
		case GUI_FNT_STYLE:
		{
			arg->font_style=(gfx_txtstyle_t)data;
			break;
		}
		case GUI_TEXT:
		{
			arg->text=(char*)data;
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

ecode_t gui_button_get( gui_obj_t *obj, gui_field_t field, void *data )
{
	gui_button_t *const arg=obj->arg;
	pstart();
	
	switch ( field )
	{
		case GUI_IMAGE:
		{
			*(char**)data=arg->img_name;
			break;
		}
		case GUI_IMG_MOUSEON:
		{
			*(char**)data=arg->img_mon_name;
			break;
		}
		case GUI_IMG_CLICK:
		{
			*(char**)data=arg->img_mdown_name;
			break;
		}
		case GUI_FONT:
		{
			*(char**)data=arg->font_name;
			break;
		}
		case GUI_FNT_COLOUR:
		{
			*(SDL_Color*)data=arg->font_colour;
			break;
		}
		case GUI_FNT_STYLE:
		{
			*(gfx_txtstyle_t*)data=arg->font_style;
			break;
		}
		case GUI_TEXT:
		{
			*(char**)data=arg->text;
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

ecode_t gui_button_load( gui_obj_t *obj )
{
	__label__ cleanup, cleanup2;
	gui_button_t *const arg=obj->arg;
	enum
	{
		LOADED_NONE,
		LOADED_IMG,
		LOADED_IMG_MON,
		LOADED_IMG_MDOWN,
		LOADED_FONT
	}	loaded=LOADED_NONE;
	pstart();
	
	arg->img=res_access(arg->img_name);
	if ( arg->img==NULL )
		goto cleanup;
	loaded=LOADED_IMG;
	
	arg->img_mon=res_access(arg->img_mon_name);
	if ( arg->img_mon==NULL )
		goto cleanup;
	loaded=LOADED_IMG_MON;
	
	arg->img_mdown=res_access(arg->img_mdown_name);
	if ( arg->img_mdown==NULL )
		goto cleanup;
	loaded=LOADED_IMG_MDOWN;
	
	arg->font=res_access(arg->font_name);
	if ( arg->font==NULL )
		goto cleanup;
	loaded=LOADED_FONT;
	pdebug("================================%p\n",arg->font);
	arg->text_face=gfx_txtrender(arg->text,arg->font,arg->font_style,arg->font_colour);
	if ( arg->text_face==NULL )
		goto cleanup2;
	
	arg->current=arg->img;
	arg->text_rx=(obj->width-arg->text_face->w)/2;
	arg->text_ry=(obj->height-arg->text_face->h)/2;
	
	if ( arg->text_rx>0 )
	{
		arg->text_x=0;
		arg->text_w=arg->text_face->w;
	}	else
	{
		arg->text_x=-arg->text_rx;
		arg->text_w=obj->width;
	}
	
	if ( arg->text_ry>0 )
	{
		arg->text_y=0;
		arg->text_h=arg->text_face->h;
	}	else
	{
		arg->text_y=-arg->text_ry;
		arg->text_h=obj->height;
	}
	
	arg->mon=0;
	arg->mdown=0;
	
	pstop();
	return E_NONE;
	
	cleanup:
	SET_ERROR(E_KU2,vstr(gettext("Cannot load a resource (kucode=%d)"),kucode));
	cleanup2:
	switch ( loaded )
	{
		case LOADED_FONT:
			res_release(arg->font_name);
		case LOADED_IMG_MDOWN:
			res_release(arg->img_mdown_name);
		case LOADED_IMG_MON:
			res_release(arg->img_mdown_name);
		case LOADED_IMG:
			res_release(arg->img_mdown_name);
		default: {}
	}
	return ecode;
}

ecode_t gui_button_uload( gui_obj_t *obj )
{
	gui_button_t *const arg=obj->arg;
	kucode_t waserror=KE_NONE;
	pstart();
	
	if ( (kucode=res_release(arg->img_name))!=KE_NONE )
		waserror=kucode;
	
	if ( (kucode=res_release(arg->img_mon_name))!=KE_NONE )
		waserror=kucode;
	
	if ( (kucode=res_release(arg->img_mdown_name))!=KE_NONE )
		waserror=kucode;
	
	if ( (kucode=res_release(arg->font_name))!=KE_NONE )
		waserror=kucode;
	
	SDL_FreeSurface(arg->text_face);
	
	if ( waserror!=KE_NONE )
	{
		SET_ERROR(E_KU2,vstr(gettext("Cannot unload a resource (kucode=%d)"),waserror));
		return E_KU2;
	}
	
	pstop();
	return E_NONE;
}

ecode_t gui_button_draw( gui_obj_t *obj, int x, int y, int w, int h )
{
	gui_button_t *const arg=obj->arg;
	int butw, buth;
	pstart();
	
	if ( gfx_draw(arg->current,screen,GFX_IMG_REAL,obj->rx,obj->ry,x,y,w,h)!=E_NONE )
		return ecode;
	
	if ( gfx_draw(arg->text_face,screen,GFX_IMG_REAL,obj->rx+arg->text_rx,
		obj->ry+arg->text_ry,arg->text_x,arg->text_y,arg->text_w,arg->text_h)!=E_NONE )
		return ecode;
	
	pstop();
	return E_NONE;
}

ecode_t gui_button_mouseon( gui_obj_t *obj, SDL_Event *event, int rx, int ry )
{
	gui_button_t *const arg=obj->arg;
	pstart();
	
	if ( !(arg->mon || arg->mdown) )
	{
		arg->current=arg->img_mon;
		gui_draw(obj,0,0,obj->width,obj->height);
		arg->mon=1;
	}
	
	pstop();
	return E_NONE;
}

ecode_t gui_button_mouseoff( gui_obj_t *obj, SDL_Event *event, int rx, int ry )
{
	gui_button_t *const arg=obj->arg;
	pstart();
	
	if ( !arg->mdown )
	{
		arg->current=arg->img;
		gui_draw(obj,0,0,obj->width,obj->height);
	}
	
	arg->mon=0;
	
	pstop();
	return E_NONE;
}

ecode_t gui_button_mousedown( gui_obj_t *obj, SDL_Event *event, int rx, int ry )
{
	gui_button_t *const arg=obj->arg;
	pstart();
	
	arg->current=arg->img_mdown;
	gui_draw(obj,0,0,obj->width,obj->height);
	arg->mdown=1;
	
	pstop();
	return E_NONE;
}

ecode_t gui_button_mouseup( gui_obj_t *obj, SDL_Event *event, int rx, int ry )
{
	gui_button_t *const arg=obj->arg;
	pstart();
	
	if ( arg->mon )
		arg->current=arg->img_mon; else
		arg->current=arg->img;
	gui_draw(obj,0,0,obj->width,obj->height);
	arg->mdown=0;
	
	pstop();
	return E_NONE;
}
