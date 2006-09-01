/***************************************************************************
 *            frame.c
 *
 *  Wed Aug 30 22:45:29 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/
 
#include <string.h>

#include "frame.h"
#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/memory.h"
#include "ku2/gettext.h"
#include "log/log.h"
#include "resmanager/res.h"
#include "gfx/gfx.h"
#include "gui/gui.h"

kucode_t frame_init( gui_obj_t *obj )
{
	gui_frame_t *const frame = (gui_frame_t*)obj->widget;
	pstart();
	
	strcpy(frame->wname, "frame");
	frame->background_name = NULL;
	frame->background = NULL;

	obj->loadf = frame_load;
	obj->uloadf = frame_uload;
	obj->enable = frame_enable;
	obj->dim = frame_dim;
	obj->set = frame_set;
	obj->get = frame_get;
	obj->draw = frame_draw;

	pstop();
	return KE_NONE;
}

kucode_t frame_load( gui_obj_t *obj )
{
	gui_frame_t *const frame = (gui_frame_t*)obj->widget;
	pstart();

	frame->background = res_access(frame->background_name);
	if ( frame->background == NULL )
	{
		plog(gettext("Object \"frame\" %u background \"%s\" was not loaded: %d\n"), \
			obj->id, frame->background_name, kucode);
		return kucode;
	}

	pstop();
	return KE_NONE;
}

kucode_t frame_uload( gui_obj_t *obj )
{
	gui_frame_t *const frame = (gui_frame_t*)obj->widget;
	pstart();

	if ( res_release(frame->background_name) != KE_NONE )
		plog(gettext("Note: Object \"frame\" %u failed to release " \
			"a background \"%s\": %d\n"), obj->id, frame->background_name, kucode);
	dfree(frame->background_name);

	pstop();
	return KE_NONE;
}

kucode_t frame_enable( gui_obj_t *obj )
{
	pstart();

	pstop();
	return KE_NONE;
}

kucode_t frame_dim( gui_obj_t *obj )
{
	pstart();

	pstop();
	return KE_NONE;
}

kucode_t frame_set( gui_obj_t *obj, int param, void *data )
{
	gui_frame_t *const frame = (gui_frame_t*)obj->widget;
	pstart();
	
	switch ( param )
	{
		case FRAME_BACKGROUND:
		{
			char *name = dmalloc(sizeof(char)*(strlen((char*)data)+1));
			if ( name == NULL )
				KU_ERRQ(KE_MEMORY);
			strcpy(name, (char*)data);
			if ( frame->background_name )
				dfree(frame->background_name);
			frame->background_name = name;
			break;
		}
	}

	pstop();
	return KE_NONE;
}

kucode_t frame_get( gui_obj_t *obj, int param, void *data )

{
	pstart();

	pstop();
	return KE_NONE;
}

kucode_t frame_draw( gui_obj_t *obj, int x, int y, int w, int h )
{
	gui_frame_t *const frame = (gui_frame_t*)obj->widget;
	pstart();

	gfx_draw(frame->background, NULL, GFX_IMG_REAL, obj->rx, obj->ry, x, y, w, h);

	pstop();
	return KE_NONE;
}
