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
#include "io/log/log.h"
#include "dp/resmanager/res.h"
//#include "gfx/gfx.h"
#include "gui/gui.h"

#define TEXT_UD_TEXT 1

kucode_t text_init( gui_obj_t *obj )
{
	gui_text_t *const widget = (gui_text_t*)obj->widget;
	pstart();

	strcpy(widget->wname, "text");

	widget->font_name = NULL;
	widget->font_style = GFX_NORMAL;
	widget->font_r = \
	widget->font_g = \
	widget->font_b = 0;

	widget->text = NULL;

	obj->destroyf = text_destroy;
	obj->load = text_load;
	obj->uload = text_uload;
	obj->update = text_update;

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

	if ( widget->text )
		dfree(widget->text);

	pstop();
	return KE_NONE;
}

kucode_t text_load( gui_obj_t *obj )
{
	gui_text_t *const widget = (gui_text_t*)obj->widget;
	pstart();

	ku_avoid( widget->font_name == NULL );
	ku_avoid( widget->text == NULL );

	widget->font = res_access(widget->font_name);
	if ( widget->font == NULL )
	{
		plogfn(gettext("Object %u font '%s` was not loaded: %d\n"), \
			obj->id, widget->font_name, kucode);
		return kucode;
	}
	widget->face = gfx_font_render(widget->text, widget->font, \
		widget->font_style, widget->font_r, widget->font_g, widget->font_b);
	if ( widget->face == NULL )
	{
		plogfn(gettext("Object %u text cannot be rendered: %d\n"), obj->id, kucode);
		res_release(widget->font_name);
		return kucode;
	}

	obj->width = widget->face->w;
	obj->height = widget->face->h;

	pstop();
	return KE_NONE;
}

kucode_t text_uload( gui_obj_t *obj )
{
	gui_text_t *const widget = (gui_text_t*)obj->widget;
	pstart();

	ku_avoid( obj->status == GUI_NOTLOADED );
	ku_avoid( widget->font_name == NULL );

	if ( res_release(widget->font_name) != KE_NONE )
		plog(gettext("Note: Object 'gfxbutton` %u failed to release " \
			"a font '%s`: %d\n"), obj->id, widget->font_name, kucode);

	if ( widget->face )
		dfree(widget->face);

	pstop();
	return KE_NONE;
}

kucode_t text_update( gui_obj_t *obj )
{
	gui_text_t *const widget = (gui_text_t*)obj->widget;
	if ( obj->updated&TEXT_UD_TEXT )
	{
		obj->updated &= ~TEXT_UD_TEXT;
		if ( widget->face )
			dfree(widget->face);
		widget->face = gfx_font_render(widget->text, widget->font, \
				widget->font_style, widget->font_r, widget->font_g, widget->font_b);
		obj->width = widget->face->w;
		obj->height = widget->face->h;
	}
	return KE_NONE;
}

kucode_t text_set( gui_obj_t *obj, int param, void *data )
{
	gui_text_t *const widget = (gui_text_t*)obj->widget;
	pstart();

	switch ( param )
	{
		case TEXT_TEXT:
		{
			char *text = dmalloc(sizeof(char)*(strlen((char*)data)+1));
			if ( text == NULL )
				KU_ERRQ(KE_MEMORY);
			strcpy(text, (char*)data);
			if ( widget->text )
				dfree(widget->text);
			widget->text = text;
			obj->updated |= TEXT_UD_TEXT;
			break;
		}
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
		case TEXT_FSTYLE:
		{
			widget->font_style = (gfx_font_style_t)data;
			break;
		}
		case TEXT_FCR:
		{
			widget->font_r = (uint8_t)data;
			break;
		}
		case TEXT_FCG:
		{
			widget->font_g = (uint8_t)data;
			break;
		}
		case TEXT_FCB:
		{
			widget->font_b = (uint8_t)data;
			break;
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

kucode_t text_draw( gui_obj_t *obj, int x, int y )
{
	gui_text_t *const widget = (gui_text_t*)obj->widget;
	pstart();

	gfx_draw(widget->face, x, y);

	pstop();
	return KE_NONE;
}
