/***************************************************************************
 *            button.h
 *
 *  Sun Jun 18 16:15:52 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#ifndef __GUI_BUTTON_H__
#define __GUI_BUTTON_H__

#include "SDL_ttf.h"

#include "gui_core.h"
#include "gfx/gfx.h"

typedef
struct
{
	int mon, mdown;
	SDL_Surface *current;
	
	char *img_name, *img_mon_name, *img_mdown_name;
	SDL_Surface *img, *img_mon, *img_mdown;
	
	char *text;
	char *font_name;
	TTF_Font *font;
	SDL_Color font_colour;
	gfx_txtstyle_t font_style;
	SDL_Surface *text_face;
	int text_rx, text_ry, text_x, text_y, text_w, text_h;
}	gui_button_t;

ecode_t gui_button_create( gui_obj_t *obj );

ecode_t gui_button_set( gui_obj_t *obj, gui_field_t field, void *data );
ecode_t gui_button_get( gui_obj_t *obj, gui_field_t field, void *data );

ecode_t gui_button_load( gui_obj_t *obj );
ecode_t gui_button_uload( gui_obj_t *obj );

ecode_t gui_button_draw( gui_obj_t *obj, int x, int y, int w, int h );

ecode_t gui_button_mouseon( gui_obj_t *obj, SDL_Event *event, int rx, int ry );
ecode_t gui_button_mouseoff( gui_obj_t *obj, SDL_Event *event, int rx, int ry );
ecode_t gui_button_mousedown( gui_obj_t *obj, SDL_Event *event, int rx, int ry );
ecode_t gui_button_mouseup( gui_obj_t *obj, SDL_Event *event, int rx, int ry );

#endif
