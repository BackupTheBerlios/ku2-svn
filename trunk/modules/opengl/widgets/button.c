/***************************************************************************
 *            gfxbut.c
 *
 *  Wed Aug 30 22:45:41 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <string.h>

#include "button.h"
#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/memory.h"
#include "ku2/gettext.h"

#include "io/log/log.h"
#include "dp/resmanager/res.h"
#include "modules/opengl/gfx/gfx.h"
#include "modules/opengl/gui/gui.h"

#define BUTTON_UD_NORM 1
#define BUTTON_UD_MON 2
#define BUTTON_UD_MDN 4
#define BUTTON_UD_FONT 8
#define BUTTON_UD_CAPTION 16

static void button_ch_state( gui_obj_t *obj, int state )
{
	gui_button_t *const widget = (gui_button_t*)obj->widget;

	switch ( state )
	{
		case BUTTON_NORM:
		{
			widget->face = widget->back_nor;
			break;
		}
		case BUTTON_MON:
		{
			widget->face = widget->back_mon ? widget->back_mon : widget->back_nor;
			break;
		}
		case BUTTON_MDN:
		{
			widget->face = widget->back_mdn ? widget->back_mdn : widget->back_nor;
			break;
		}
	}

	obj->width = widget->face->w;
	obj->height = widget->face->h;

	if ( widget->fontface )
	{
		widget->font_rx = (obj->width-widget->fontface->w)/2;
		widget->font_ry = (obj->height-widget->fontface->h)/2;
	}
}

kucode_t button_init( gui_obj_t *obj )
{
	gui_button_t *const widget = (gui_button_t*)obj->widget;
	pstart();

	strcpy(widget->wname, "button");
	widget->st_mon = 0;
	widget->st_mdown = 0;

	widget->click = NULL;
	widget->back_nor_name = NULL;
	widget->back_mon_name = NULL;
	widget->back_mdn_name = NULL;
	widget->font_name = NULL;

	widget->fontface = NULL;

	widget->font_r = \
	widget->font_g = \
	widget->font_b = 0;

	widget->caption = NULL;

	obj->destroyf = button_destroy;
	obj->load = button_load;
	obj->uload = button_uload;
	obj->update = button_update;

	obj->set = button_set;
	obj->get = button_get;

	obj->mon = button_mon;
	obj->moff = button_moff;
	obj->mdown = button_mdown;
	obj->mup = button_mup;

	obj->draw = button_draw;

	pstop();
	return KE_NONE;
}

kucode_t button_destroy( gui_obj_t *obj )
{
	gui_button_t *const widget = (gui_button_t*)obj->widget;
	pstart();

	if ( obj->status > GUI_NOTLOADED )
		button_uload(obj);

	if ( widget->back_nor_name )
		dfree(widget->back_nor_name);

	if ( widget->back_mon_name )
		dfree(widget->back_mon_name);

	if ( widget->back_mdn_name )
		dfree(widget->back_mdn_name);

	if ( widget->font_name )
		dfree(widget->font_name);

	if ( widget->caption )
		dfree(widget->caption);

	pstop();
	return KE_NONE;
}

//	загрузка данных кнопки
kucode_t button_load( gui_obj_t *obj )
{
	gui_button_t *const widget = (gui_button_t*)obj->widget;
	pstart();

	ku_avoid( obj->status != GUI_NOTLOADED );

	//	загружаем главное изображение, не может быть NULL
	ku_avoid( widget->back_nor_name == NULL );
	obj->updated = BUTTON_UD_NORM;

	/*
		загружаем изображение активной кнопки,
		если NULL, то используется нормальное изображение
	*/
	if ( widget->back_mon_name )
		obj->updated |= BUTTON_UD_MON; else
		widget->back_mon = NULL;

	/*
		загружаем изображение нажатой кнопки,
		если NULL, то используется нормальное изображение
	*/
	if ( widget->back_mdn_name )
		obj->updated |= BUTTON_UD_MDN; else
		widget->back_mdn = NULL;

	/*
		загружаем шрифт текста кнопки,
		если NULL, то кнопка не будет иметь текста
	*/
	if ( widget->font_name )
	{
		obj->updated |= BUTTON_UD_FONT;
		//	если установлен текст кнопки..
		if ( widget->caption )
			obj->updated |= BUTTON_UD_CAPTION; else
			widget->fontface = NULL;
	}	else
	{
		widget->font = NULL;
		widget->fontface = NULL;
	}

	if ( button_update(obj) != KE_NONE )
		return kucode;
	button_ch_state(obj, BUTTON_NORM);

	pstop();
	return KE_NONE;
}

//	освобождение от загруженных данных
kucode_t button_uload( gui_obj_t *obj )
{
	gui_button_t *const widget = (gui_button_t*)obj->widget;
	pstart();

	ku_avoid( obj->status == GUI_NOTLOADED );

	ku_avoid( widget->back_nor_name == NULL );
	if ( res_release(widget->back_nor_name) != KE_NONE )
		plogfn(gettext("Object %u failed to release a background \"%s\": %d\n"), \
			obj->id, widget->back_nor_name, kucode);

	if ( widget->back_mon && (res_release(widget->back_mon_name) != KE_NONE) )
		plogfn(gettext("Object %u failed to release a mouse-on background \"%s\": %d\n"), \
			obj->id, widget->back_mon_name, kucode);

	if ( widget->back_mdn && (res_release(widget->back_mdn_name) != KE_NONE) )
		plogfn(gettext("Object %u failed to release a mouse-down background \"%s\": %d\n"), \
			obj->id, widget->back_mdn_name, kucode);

	if ( widget->font && (res_release(widget->font_name) != KE_NONE) )
		plogfn(gettext("Object %u failed to release a font \"%s\": %d\n"), \
			obj->id, widget->font_name, kucode);

	if ( widget->fontface )
		dfree(widget->fontface);

	pstop();
	return KE_NONE;
}

kucode_t button_update( gui_obj_t *obj )
{
	gui_button_t *const widget = (gui_button_t*)obj->widget;

	if ( obj->updated&BUTTON_UD_NORM )
	{
		obj->updated &= ~BUTTON_UD_NORM;
		widget->back_nor = res_access(widget->back_nor_name);
		if ( widget->back_nor == NULL )
		{
			plogfn(gettext("Object %u background '%s` was not loaded: %d\n"), \
				obj->id, widget->back_nor_name, kucode);
			return kucode;
		}
	}

	if ( obj->updated&BUTTON_UD_MON )
	{
		obj->updated &= ~BUTTON_UD_MON;
		widget->back_mon = res_access(widget->back_mon_name);
		if ( widget->back_mon == NULL )
		{
			plogfn(gettext("Object %u mouse-on background '%s` was not loaded: %d\n"), \
				obj->id, widget->back_mon_name, kucode);
		}
	}

	if ( obj->updated&BUTTON_UD_MDN )
	{
		obj->updated &= ~BUTTON_UD_MDN;
		widget->back_mdn = res_access(widget->back_mdn_name);
		if ( widget->back_mdn == NULL )
		{
			plogfn(gettext("Object %u mouse-down background '%s` was not loaded: %d\n"), \
				obj->id, widget->back_mdn_name, kucode);
		}
	}

	if ( obj->updated&BUTTON_UD_FONT )
	{
		obj->updated &= ~BUTTON_UD_FONT;
		widget->font = res_access(widget->font_name);
		if ( widget->font == NULL )
		{
			plogfn(gettext("Object %u font '%s` was not loaded: %d\n"), \
				obj->id, widget->font_name, kucode);
		}
	}

	if ( obj->updated&BUTTON_UD_CAPTION )
	{
		obj->updated &= ~BUTTON_UD_CAPTION;
		if ( widget->fontface )
			dfree(widget->fontface);
		widget->fontface = gfx_font_render(widget->caption, widget->font, \
			widget->font_style, widget->font_r, widget->font_g, widget->font_b);
		if ( widget->fontface )
		{
			widget->font_rx = (obj->width-widget->fontface->w)/2;
			widget->font_ry = (obj->height-widget->fontface->h)/2;
		}
	}

	return KE_NONE;
}

kucode_t button_set( gui_obj_t *obj, int param, void *data )
{
	gui_button_t *const widget = (gui_button_t*)obj->widget;
	pstart();

	switch ( param )
	{
		case BUTTON_NORM:
		{
			char *name = dmalloc(sizeof(char)*(strlen((char*)data)+1));
			if ( name == NULL )
				KU_ERRQ(KE_MEMORY);
			strcpy(name, (char*)data);
			if ( widget->back_nor_name )
				dfree(widget->back_nor_name);
			widget->back_nor_name = name;
			break;
		}
		case BUTTON_MON:
		{
			char *name = dmalloc(sizeof(char)*(strlen((char*)data)+1));
			if ( name == NULL )
				KU_ERRQ(KE_MEMORY);
			strcpy(name, (char*)data);
			if ( widget->back_mon_name )
				dfree(widget->back_mon_name);
			widget->back_mon_name = name;
			break;
		}
		case BUTTON_MDN:
		{
			char *name = dmalloc(sizeof(char)*(strlen((char*)data)+1));
			if ( name == NULL )
				KU_ERRQ(KE_MEMORY);
			strcpy(name, (char*)data);
			if ( widget->back_mdn_name )
				dfree(widget->back_mdn_name);
			widget->back_mdn_name = name;
			break;
		}
		case BUTTON_CLICK:
		{
			widget->click = (gui_cb_f)data;
			break;
		}
		case BUTTON_CAPTION:
		{
			char *caption = dmalloc(sizeof(char)*(strlen((char*)data)+1));
			if ( caption == NULL )
				KU_ERRQ(KE_MEMORY);
			strcpy(caption, (char*)data);
			if ( widget->caption )
				dfree(widget->caption);
			widget->caption = caption;
			obj->updated |= BUTTON_UD_CAPTION;
			break;
		}
		case BUTTON_FONT:
		{
			char *name = dmalloc(sizeof(char)*(strlen((char*)data)+1));
			if ( name == NULL )
				KU_ERRQ(KE_MEMORY);
			strcpy(name, (char*)data);
			if ( widget->font_name )
				dfree(widget->font_name);
			widget->font_name = name;
			obj->updated |= BUTTON_UD_CAPTION|BUTTON_UD_FONT;
			break;
		}
		case BUTTON_FSTYLE:
		{
			widget->font_style = (gfx_font_style_t)data;
			break;
		}
		case BUTTON_FCR:
		{
			widget->font_r = (uint8_t)data;
			break;
		}
		case BUTTON_FCG:
		{
			widget->font_g = (uint8_t)data;
			break;
		}
		case BUTTON_FCB:
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

kucode_t button_get( gui_obj_t *obj, int param, void *data )
{
	gui_button_t *const widget = (gui_button_t*)obj->widget;
	pstart();

	switch ( param )
	{
		case BUTTON_NORM:
		{
			*((char**)data) = widget->back_nor_name;
			break;
		}
		case BUTTON_MON:
		{
			*((char**)data) = widget->back_mon_name;
			break;
		}
		case BUTTON_MDN:
		{
			*((char**)data) = widget->back_mdn_name;
			break;
		}
		case BUTTON_CLICK:
		{
			*((gui_cb_f*)data) = widget->click;
			break;
		}
		case BUTTON_CAPTION:
		{
			*((char**)data) = widget->caption;
			break;
		}
		case BUTTON_FONT:
		{
			*((char**)data) = widget->font_name;
			break;
		}
		case BUTTON_FSTYLE:
		{
			*((gfx_font_style_t*)data) = widget->font_style;
			break;
		}
		case BUTTON_FCR:
		{
			*((uint8_t*)data) = widget->font_r;
			break;
		}
		case BUTTON_FCG:
		{
			*((uint8_t*)data) = widget->font_g;
			break;
		}
		case BUTTON_FCB:
		{
			*((uint8_t*)data) = widget->font_b;
			break;
		}
		default:
			KU_ERRQ(KE_INVALID);
	}

	pstop();
	return KE_NONE;
}

gui_event_st button_mon( gui_obj_t *obj, int x, int y, int z )
{
	gui_button_t *const widget = (gui_button_t*)obj->widget;
	pstart();

	if ( !widget->st_mdown && !widget->st_mon )
		button_ch_state(obj, BUTTON_MON);

	widget->st_mon = 1;

	pstop();
	return GUIE_EAT;
}

gui_event_st button_moff( gui_obj_t *obj, int x, int y, int z )
{
	gui_button_t *const widget = (gui_button_t*)obj->widget;
	pstart();

	if ( !widget->st_mdown )
		button_ch_state(obj, BUTTON_NORM);

	widget->st_mon = 0;

	pstop();
	return GUIE_EAT;
}

gui_event_st button_mdown( gui_obj_t *obj, int x, int y, int z )
{
	gui_button_t *const widget = (gui_button_t*)obj->widget;
	pstart();

	button_ch_state(obj, BUTTON_MDN);
	widget->st_mdown = 1;

	pstop();
	return GUIE_EAT;
}

gui_event_st button_mup( gui_obj_t *obj, int x, int y, int z )
{
	gui_button_t *const widget = (gui_button_t*)obj->widget;
	pstart();

	widget->st_mdown = 0;
	if ( widget->st_mon )
		button_ch_state(obj, BUTTON_MON); else
		button_ch_state(obj, BUTTON_NORM);

	if ( widget->click && (widget->click(obj, (void*)z) != KE_NONE) )
		return GUIE_ERROR;

	pstop();
	return GUIE_EAT;
}

kucode_t button_draw( gui_obj_t *obj, int x, int y )
{
	gui_button_t *const widget = (gui_button_t*)obj->widget;
	pstart();

	if ( gfx_draw(widget->face, x, y) != KE_NONE )
		return kucode;

	if ( widget->fontface && (gfx_draw(widget->fontface, x+widget->font_rx, y+widget->font_ry) != KE_NONE) )
		return kucode;

	pstop();
	return KE_NONE;
}
