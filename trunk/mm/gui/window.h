/***************************************************************************
 *            window.h
 *
 *  Sat Jun  3 17:13:04 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#ifndef __GUI_WINDOW_H__
#define __GUI_WINDOW_H__

#include "gui_core.h"

typedef
struct
{
	char *img_name;
	SDL_Surface *img;
}	gui_window_t;

ecode_t gui_window_create( gui_obj_t *obj );

ecode_t gui_window_set( gui_obj_t *obj, gui_field_t field, void *data );
ecode_t gui_window_get( gui_obj_t *obj, gui_field_t field, void *data );

ecode_t gui_window_load( gui_obj_t *obj );
ecode_t gui_window_uload( gui_obj_t *obj );

ecode_t gui_window_draw( gui_obj_t *obj, int x, int y, int w, int h );

#endif
