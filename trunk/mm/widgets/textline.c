/***************************************************************************
 *            textline.c
 *
 *  Wed Aug 30 22:46:08 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <string.h>

#include "textline.h"
#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/memory.h"
#include "ku2/gettext.h"
#include "log/log.h"
#include "resmanager/res.h"
#include "gfx/gfx.h"
#include "gui/gui.h"

kucode_t text_init( gui_obj_t *obj )
{
	gui_text_t *const widget = (gui_text_t*)obj->widget;
	pstart();
	
	strcpy(widget->wname, "textline");
	widget->font_name = NULL;
	widget->flags = GUI_FONT_ZERO;

	obj->destroyf = text_destroy;
	obj->load = text_load;
	obj->uload = text_uload;
	obj->enable = NULL;
	obj->disable = NULL;
	obj->hide = NULL;

	//obj->dim = text_dim;

	obj->set = text_set;
	obj->get = text_get;

	obj->draw = text_draw;

	pstop();
	return KE_NONE;
}

kucode_t text_destroy( gui_obj_t *obj )
{
	gui_text_t *const widget = (gui_text_t*)obj->widget;
	pstart();

	if ( obj->status > GUI_NOTLOADED )
		text_uload(obj);

	if ( widget->font_name )
		dfree(widget->font_name);

	pstop();
	return KE_NONE;
}

kucode_t text_load( gui_obj_t *obj )
{
	gui_text_t *const widget = (gui_text_t*)obj->widget;
	pstart();

	widget->font = res_access(widget->font_name);
	if ( widget->font == NULL )
	{
		plog(gettext("Object 'textline` %u font '%s` was not loaded: %d\n"), \
			obj->id, widget->font_name, kucode);
		return kucode;
	}

	//!!!!!!!
	widget->back = NULL;

	pstop();
	return KE_NONE;
}

kucode_t text_uload( gui_obj_t *obj )
{
	gui_text_t *const widget = (gui_text_t*)obj->widget;
	pstart();

	if ( res_release(widget->font_name) != KE_NONE )
		plog(gettext("Note: Object 'gfxbutton` %u failed to release " \
			"a font '%s`: %d\n"), obj->id, widget->font_name, kucode);

	SDL_FreeSurface(widget->back);

	pstop();
	return KE_NONE;
}

kucode_t text_set( gui_obj_t *obj, int param, void *data )
{
	gui_text_t *const widget = (gui_text_t*)obj->widget;
	pstart();
	
	switch ( param )
	{
		case TEXT_FONT:
		{
			char *name = dmalloc(sizeof(char)*(strlen((char*)data)+1));
			if ( name == NULL )
				KU_ERRQ(KE_MEMORY);
			strcpy(name, (char*)data);
			if ( widget->font_name )
				dfree(widget->font_name);
			widget->font_name = name;
			break;
		}
		case TEXT_FLAGS:
		{
			widget->flags = (ku_flag32_t)data;
			break;
		}
		case TEXT_TEXT:
		{
			char *text = dmalloc(sizeof(char)*(strlen((char*)data)+1));
			if ( text == NULL )
				KU_ERRQ(KE_MEMORY);
			strcpy(text, (char*)data);
			if ( widget->text )
				dfree(widget->text);
			widget->text = text;
		}
		default:
			KU_ERRQ(KE_INVALID);
	}

	pstop();
	return KE_NONE;
}

kucode_t text_get( gui_obj_t *obj, int param, void *data )
{
	gui_text_t *const widget = (gui_text_t*)obj->widget;
	pstart();
/*
	switch ( param )
	{
		case GFXBUT_NORM:
		{
			*((char**)data) = widget->back_nor_name;
			break;
		}
		case GFXBUT_MON:
		{
			*((char**)data) = widget->back_mon_name;
			break;
		}
		case GFXBUT_MDN:
		{
			*((char**)data) = widget->back_mdn_name;
			break;
		}
		case GFXBUT_CLICK:
		{
			*((gui_cb_f*)data) = widget->click;
			break;
		}
		default:
			KU_ERRQ(KE_INVALID);
	}
*/
	pstop();
	return KE_NONE;
}

kucode_t text_draw( gui_obj_t *obj, int x, int y, int w, int h )
{
	gui_text_t *const widget = (gui_text_t*)obj->widget;
	pstart();

	if ( gfx_draw(widget->back, NULL, GFX_IMG_REAL, obj->rx, \
		obj->ry, x, y, w, h) != KE_NONE )
		return kucode;

	pstop();
	return KE_NONE;
}
