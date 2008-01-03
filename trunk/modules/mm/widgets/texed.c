/*
		texed.c
		Tue Dec 25 16:28:43 2007

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2007
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

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
	
	preturn KE_NONE;
}

kucode_t texed_destroy( gui_obj_t *obj )
{
	gui_texed_t *const widget = (gui_texed_t*)obj->widget;
	pstart();
	
	KU_WITHOUT_ERROR_START();
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
	KU_WITHOUT_ERROR_STOP();
	
	KU_ERRQ_BLOCKED();
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
		plogfn_i("TEXED", gettext("Object %u background '%s` was not loaded: %d\n"), \
			obj->id, widget->back_nor_name, KU_GET_ERROR());
		KU_ERRQ_PASS();
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
			plogfn_i("TEXED", gettext("Object %u focused background '%s` was not loaded: %d\n"), \
				obj->id, widget->back_focus_name, KU_GET_ERROR());
			KU_WITHOUT_ERROR(res_release(widget->back_nor_name));
			KU_ERRQ_PASS();
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
			plogfn_i("TEXED", gettext("Object %u font '%s` was not loaded: %d\n"), \
				obj->id, widget->font_name, KU_GET_ERROR());
			KU_WITHOUT_ERROR_START();
				res_release(widget->back_nor_name);
				res_release(widget->back_focus_name);
			KU_WITHOUT_ERROR_STOP();
			KU_ERRQ_PASS();
		}	else
		//	если установлен текст кнопки..
		if ( widget->text )
		{
			widget->fontface = gfx_font_render(widget->text, widget->font, \
				widget->font_style, widget->font_r, widget->font_g, widget->font_b);
			if ( widget->fontface == NULL )
			{
				plogfn_i("TEXED", gettext("Object %u text was not rendered: %d\n"),
						 obj->id, KU_GET_ERROR());
				KU_WITHOUT_ERROR_START();
					res_release(widget->back_nor_name);
					res_release(widget->back_focus_name);
					res_release(widget->font_name);
				KU_WITHOUT_ERROR_STOP();
				KU_ERRQ_PASS();
			}
		}	else
			widget->fontface = NULL;
	}	else
	{
		widget->font = NULL;
		widget->fontface = NULL;
	}
	
	texed_ch_state(obj, TEXED_NORM);
	
	preturn KE_NONE;
}

//	освобождение от загруженных данных
kucode_t texed_uload( gui_obj_t *obj )
{
	gui_texed_t *const widget = (gui_texed_t*)obj->widget;
	pstart();
	
	ku_avoid( obj->status == GUI_NOTLOADED );
	ku_avoid( widget->back_nor_name == NULL );
	
	KU_WITHOUT_ERROR_START();
	if ( res_release(widget->back_nor_name) != KE_NONE )
		plogfn_i("TEXED", gettext("Object %u failed to release a background '%s`: %d\n"), \
			obj->id, widget->back_nor_name, KU_GET_ERROR());
	
	if ( widget->back_focus && (res_release(widget->back_focus_name) != KE_NONE) )
		plogfn_i("TEXED", gettext("Object %u failed to release a focused background '%s`: %d\n"), \
			obj->id, widget->back_focus_name, KU_GET_ERROR());
	
	if ( widget->font && (res_release(widget->font_name) != KE_NONE) )
		plogfn_i("TEXED", gettext("Object %u failed to release a font '%s`: %d\n"), \
			obj->id, widget->font_name, KU_GET_ERROR());
	
	if ( widget->fontface )
		gfx_img_free(widget->fontface);
	KU_WITHOUT_ERROR_STOP();
	
	KU_ERRQ_BLOCKED();
}

kucode_t texed_update( gui_obj_t *obj )
{
	gui_texed_t *const widget = (gui_texed_t*)obj->widget;
	pstart();
	
	if ( obj->updated&TEXED_UD_TEXT )
	{
		gfx_image_t *const oldff = widget->fontface;
		
		obj->updated &= ~TEXED_UD_TEXT;
		
		widget->fontface = gfx_font_render(widget->text, widget->font, \
			widget->font_style, widget->font_r, widget->font_g, widget->font_b);
		if ( widget->fontface == NULL )
		{
			plogfn_i("TEXED", gettext("Object %u text was not rendered: %d\n"),
					 obj->id, KU_GET_ERROR());
			widget->fontface = oldff;
			KU_ERRQ_PASS();
		}	else
			gfx_img_free(oldff);
	}
	
	preturn KE_NONE;
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
	
	preturn KE_NONE;
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
	
	preturn KE_NONE;
}

gui_event_st texed_mdown( gui_obj_t *obj, int x, int y, int z )
{
	//gui_texed_t *const widget = (gui_texed_t*)obj->widget;
	pstart();
	
	if ( gui_focus(obj) != KE_NONE )
		preturn GUIE_ERROR;
	
	texed_ch_state(obj, TEXED_FOCUS);
	
	preturn GUIE_EAT;
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
			KU_WITHOUT_ERROR(texed_update(obj));
		}
	}	else
	if ( ch == SDLK_BACKSPACE )
	{
		if ( widget->textpos > 0 )
		{
			widget->text[--widget->textpos] = 0;
			obj->updated |= TEXED_UD_TEXT;
			KU_WITHOUT_ERROR(texed_update(obj));
		}
	}
	
	preturn GUIE_EAT;
}

gui_event_st texed_defocus( gui_obj_t *obj )
{
	pstart();
	texed_ch_state(obj, TEXED_NORM);
	preturn GUIE_EAT;
}

kucode_t texed_draw( gui_obj_t *obj, int x, int y )
{
	gui_texed_t *const widget = (gui_texed_t*)obj->widget;
	pstart();
	
	if ( gfx_img_draw(widget->face, x, y) != KE_NONE )
		KU_ERRQ_PASS();
	
	if ( widget->fontface && (gfx_img_draw(widget->fontface, x+10, y+(widget->face->h-widget->fontface->h)/2) != KE_NONE) )
		KU_ERRQ_PASS();
	
	preturn KE_NONE;
}
