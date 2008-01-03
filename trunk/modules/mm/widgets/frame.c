/*
		frame.c
		Tue Dec 25 16:03:52 2007

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2007
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

#include <string.h>

#include "frame.h"
#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/memory.h"
#include "ku2/gettext.h"
#include "io/log/log.h"
#include "dp/resmanager/res.h"
#include "gui/gui.h"

kucode_t frame_init( gui_obj_t *obj )
{
	gui_frame_t *const frame = (gui_frame_t*)obj->widget;
	pstart();
	
	strcpy(frame->wname, "frame");
	frame->background_name = NULL;
	frame->background = NULL;
	
	obj->destroyf = frame_destroy;
	obj->load = frame_load;
	obj->uload = frame_uload;
	
	obj->dim = NULL;
	
	obj->set = frame_set;
	obj->get = frame_get;
	
	obj->draw = frame_draw;
	
	preturn KE_NONE;
}

kucode_t frame_destroy( gui_obj_t *obj )
{
	gui_frame_t *const frame = (gui_frame_t*)obj->widget;
	pstart();
	
	if ( obj->status > GUI_NOTLOADED )
		frame_uload(obj);
	
	if ( frame->background_name )
		dfree(frame->background_name);
	
	preturn KE_NONE;
}

kucode_t frame_load( gui_obj_t *obj )
{
	gui_frame_t *const frame = (gui_frame_t*)obj->widget;
	pstart();
	
	ku_avoid( frame->background_name == NULL );
	
	frame->background = res_access(frame->background_name);
	if ( frame->background == NULL )
	{
		plog(gettext("Object 'frame` %u background '%s` was not loaded: %d\n"), \
			obj->id, frame->background_name, KU_GET_ERROR());
		KU_ERRQ_PASS();
	}
	obj->width = frame->background->w;
	obj->height = frame->background->h;
	
	preturn KE_NONE;
}

kucode_t frame_uload( gui_obj_t *obj )
{
	gui_frame_t *const frame = (gui_frame_t*)obj->widget;
	pstart();
	
	ku_avoid( frame->background == NULL );
	
	KU_WITHOUT_ERROR_START();
		if ( res_release(frame->background_name) != KE_NONE )
			plog(gettext("Note: Object 'frame` %u failed to release " \
				"a background '%s`: %d\n"), obj->id, frame->background_name, KU_GET_ERROR());
		frame->background = NULL;
	KU_WITHOUT_ERROR_STOP();
	
	KU_ERRQ_BLOCKED();
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
		default:
			KU_ERRQ(KE_INVALID);
	}
	
	preturn KE_NONE;
}

kucode_t frame_get( gui_obj_t *obj, int param, void *data )
{
	gui_frame_t *const frame = (gui_frame_t*)obj->widget;
	pstart();
	
	switch ( param )
	{
		case FRAME_BACKGROUND:
		{
			*((char**)data) = frame->background_name;
			break;
		}
		default:
			KU_ERRQ(KE_INVALID);
	}
	
	preturn KE_NONE;
}

kucode_t frame_draw( gui_obj_t *obj, int x, int y )
{
	gui_frame_t *const frame = (gui_frame_t*)obj->widget;
	pstart();
	
	if ( gfx_img_draw(frame->background, x, y) != KE_NONE )
		KU_ERRQ_PASS();
	
	preturn KE_NONE;
}
