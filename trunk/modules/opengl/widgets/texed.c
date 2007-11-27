/***************************************************************************
 *            texted.c
 *
 *  Wed Aug 30 22:45:58 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <string.h>

#include "ku2/debug.h"
#include "ku2/memory.h"
#include "ku2/gettext.h"
#include "io/log/log.h"
#include "dp/resmanager/res.h"

#include "texed.h"
//#include "gfx/gfx.h"
#include "gui/gui.h"

#define TEXED_UD_TEXT 1

static void texed_ch_state( gui_obj_t *obj, int state )
{
	gui_texed_t *const widget = (gui_texed_t*)obj->widget;

	switch ( state )
	{
		case TEXED_NORM:
		{
			widget->face = widget->back_nor;
			break;
		}
		case TEXED_FOCUS:
		{
			widget->face = widget->back_focus ? widget->back_focus : widget->back_nor;
			break;
		}
	}

	obj->width = widget->face->w;
	obj->height = widget->face->h;
}

kucode_t texed_init( gui_obj_t *obj )
{
	gui_texed_t *const widget = (gui_texed_t*)obj->widget;
	pstart();

	strcpy(widget->wname, "texed");

	widget->keypress = NULL;
	widget->back_nor_name = NULL;
	widget->back_focus_name = NULL;
	widget->font_name = NULL;

	widget->font_style = GFX_NORMAL;
	widget->font_r = \
	widget->font_g = \
	widget->font_b = 0;

	widget->textlen = -1;
	widget->textpos = 0;
	widget->text = NULL;

	obj->destroyf = texed_destroy;
	obj->load = texed_load;
	obj->uload = texed_uload;

	obj->set = texed_set;
	obj->get = texed_get;

	obj->mdown = texed_mdown;
	obj->kdown = texed_kdown;
	obj->defocus = texed_defocus;

	obj->draw = texed_draw;

	pstop();
	return KE_NONE;
}

kucode_t texed_destroy( gui_obj_t *obj )
{
	gui_texed_t *const widget = (gui_texed_t*)obj->widget;
	pstart();

	if ( obj->status > GUI_NOTLOADED )
		texed_uload(obj);

	if ( widget->back_nor_name )
		dfree(widget->back_nor_name);

	if ( widget->back_focus_name )
		dfree(widget->back_focus_name);

	if ( widget->font_name )
		dfree(widget->font_name);

	if ( widget->text )
		dfree(widget->text);

	pstop();
	return KE_NONE;
}

//	загрузка данных кнопки
kucode_t texed_load( gui_obj_t *obj )
{
	gui_texed_t *const widget = (gui_texed_t*)obj->widget;
	pstart();

	ku_avoid( obj->status != GUI_NOTLOADED );

	//	загружаем главное изображение, не может быть NULL
	ku_avoid( widget->back_nor_name == NULL );
	widget->back_nor = res_access(widget->back_nor_name);
	if ( widget->back_nor == NULL )
	{
		plogfn(gettext("Object %u background '%s` was not loaded: %d\n"), \
			obj->id, widget->back_nor_name, kucode);
		return kucode;
	}

	/*
		загружаем изображение активной кнопки,
		если NULL, то используется нормальное изображение
	*/
	if ( widget->back_focus_name )
	{
		widget->back_focus = res_access(widget->back_focus_name);
		if ( widget->back_focus == NULL )
		{
			plogfn(gettext("Object %u focused background '%s` was not loaded: %d\n"), \
				obj->id, widget->back_focus_name, kucode);
		}
	}	else
		widget->back_focus = NULL;

	/*
		загружаем шрифт текста кнопки,
		если NULL, то кнопка не будет иметь текста
	*/
	if ( widget->font_name )
	{
		widget->font = res_access(widget->font_name);
		if ( widget->font == NULL )
		{
			plogfn(gettext("Object %u font '%s` was not loaded: %d\n"), \
				obj->id, widget->font_name, kucode);
		}	else
		//	если установлен текст кнопки..
		if ( widget->text )
		{
			widget->fontface = gfx_font_render(widget->text, widget->font, \
				widget->font_style, widget->font_r, widget->font_g, widget->font_b);
		}	else
			widget->fontface = NULL;
	}	else
	{
		widget->font = NULL;
		widget->fontface = NULL;
	}

	texed_ch_state(obj, TEXED_NORM);

	pstop();
	return KE_NONE;
}

//	освобождение от загруженных данных
kucode_t texed_uload( gui_obj_t *obj )
{
	gui_texed_t *const widget = (gui_texed_t*)obj->widget;
	pstart();

	ku_avoid( obj->status == GUI_NOTLOADED );

	ku_avoid( widget->back_nor_name == NULL );
	if ( res_release(widget->back_nor_name) != KE_NONE )
		plogfn(gettext("Object %u failed to release a background \"%s\": %d\n"), \
			obj->id, widget->back_nor_name, kucode);

	if ( widget->back_focus && (res_release(widget->back_focus_name) != KE_NONE) )
		plogfn(gettext("Object %u failed to release a focused background \"%s\": %d\n"), \
			obj->id, widget->back_focus_name, kucode);

	if ( widget->font && (res_release(widget->font_name) != KE_NONE) )
		plogfn(gettext("Object %u failed to release a font \"%s\": %d\n"), \
			obj->id, widget->font_name, kucode);

	if ( widget->fontface )
		dfree(widget->fontface);

	pstop();
	return KE_NONE;
}

kucode_t texed_update( gui_obj_t *obj )
{
	gui_texed_t *const widget = (gui_texed_t*)obj->widget;
	if ( obj->updated&TEXED_UD_TEXT )
	{
		obj->updated &= ~TEXED_UD_TEXT;
		if ( widget->fontface )
			dfree(widget->fontface);
		widget->fontface = gfx_font_render(widget->text, widget->font, \
				widget->font_style, widget->font_r, widget->font_g, widget->font_b);
	}
	return KE_NONE;
}

kucode_t texed_set( gui_obj_t *obj, int param, void *data )
{
	gui_texed_t *const widget = (gui_texed_t*)obj->widget;
	pstart();

	switch ( param )
	{
		case TEXED_NORM:
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
		case TEXED_FOCUS:
		{
			char *name = dmalloc(sizeof(char)*(strlen((char*)data)+1));
			if ( name == NULL )
				KU_ERRQ(KE_MEMORY);
			strcpy(name, (char*)data);
			if ( widget->back_focus_name )
				dfree(widget->back_focus_name);
			widget->back_focus_name = name;
			break;
		}
		case TEXED_KP:
		{
			widget->keypress = (gui_cb_f)data;
			break;
		}
		case TEXED_TEXTLEN:
		{
			char *text = dmalloc(sizeof(char)*((widget->textlen = (int)data)+1));
			if ( text == NULL )
				KU_ERRQ(KE_MEMORY);
			if ( widget->text )
				dfree(widget->text);
			widget->text = text;
			*text = 0;
			obj->updated |= TEXED_UD_TEXT;
			break;
		}
		case TEXED_TEXT:
		{
			widget->textpos = strlen((char*)data);
			if ( widget->textpos > widget->textlen )
				KU_ERRQ(KE_FULL);
			strcpy(widget->text, (char*)data);
			obj->updated |= TEXED_UD_TEXT;
			break;
		}
		case TEXED_FONT:
		{
			char *name = dmalloc(sizeof(char)*(strlen((char*)data)+1));
			if ( name == NULL )
				KU_ERRQ(KE_MEMORY);
			strcpy(name, (char*)data);
			if ( widget->font_name )
				dfree(widget->font_name);
			widget->font_name = name;
			obj->updated |= TEXED_UD_TEXT;
			break;
		}
		case TEXED_FSTYLE:
		{
			widget->font_style = (gfx_font_style_t)data;
			obj->updated |= TEXED_UD_TEXT;
			break;
		}
		case TEXED_FCR:
		{
			widget->font_r = (uint8_t)data;
			obj->updated |= TEXED_UD_TEXT;
			break;
		}
		case TEXED_FCG:
		{
			widget->font_g = (uint8_t)data;
			obj->updated |= TEXED_UD_TEXT;
			break;
		}
		case TEXED_FCB:
		{
			widget->font_b = (uint8_t)data;
			obj->updated |= TEXED_UD_TEXT;
			break;
		}
		default:
			KU_ERRQ(KE_INVALID);
	}

	pstop();
	return KE_NONE;
}

kucode_t texed_get( gui_obj_t *obj, int param, void *data )
{
	gui_texed_t *const widget = (gui_texed_t*)obj->widget;
	pstart();

	switch ( param )
	{
		case TEXED_TEXT:
		{
			*((char**)data) = widget->text;
			break;
		}
	}/*
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
	}*/

	pstop();
	return KE_NONE;
}

gui_event_st texed_mdown( gui_obj_t *obj, int x, int y, int z )
{
	//gui_texed_t *const widget = (gui_texed_t*)obj->widget;
	pstart();

	if ( gui_focus(obj) != KE_NONE )
		return GUIE_ERROR;

	texed_ch_state(obj, TEXED_FOCUS);

	pstop();
	return GUIE_EAT;
}

gui_event_st texed_kdown( gui_obj_t *obj, char ch )
{
	gui_texed_t *const widget = (gui_texed_t*)obj->widget;
	pstart();

	if ( isalnum(ch) || (ch == ' ') )
	{
		if ( widget->textpos < widget->textlen )
		{
			widget->text[widget->textpos++] = ch;
			widget->text[widget->textpos] = 0;
			obj->updated |= TEXED_UD_TEXT;
			texed_update(obj);
		}
	}	else
	if ( ch == SDLK_BACKSPACE )
	{
		if ( widget->textpos > 0 )
		{
			widget->text[--widget->textpos] = 0;
			obj->updated |= TEXED_UD_TEXT;
			texed_update(obj);
		}
	}

	pstop();
	return GUIE_EAT;
}

gui_event_st texed_defocus( gui_obj_t *obj )
{
	texed_ch_state(obj, TEXED_NORM);
	return GUIE_EAT;
}

kucode_t texed_draw( gui_obj_t *obj, int x, int y )
{
	gui_texed_t *const widget = (gui_texed_t*)obj->widget;
	pstart();

	if ( gfx_draw(widget->face, x, y) != KE_NONE )
		return kucode;

	if ( widget->fontface && (gfx_draw(widget->fontface, x+10, y+(widget->face->h-widget->fontface->h)/2) != KE_NONE) )
		return kucode;

	pstop();
	return KE_NONE;
}
