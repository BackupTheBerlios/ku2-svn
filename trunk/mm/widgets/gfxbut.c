/***************************************************************************
 *            gfxbut.c
 *
 *  Wed Aug 30 22:45:41 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <string.h>

#include "gfxbut.h"
#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/memory.h"
#include "ku2/gettext.h"
#include "log/log.h"
#include "resmanager/res.h"
#include "gfx/gfx.h"
#include "gui/gui.h"

kucode_t gfxbut_init( gui_obj_t *obj )
{
	gui_gfxbut_t *const widget = (gui_gfxbut_t*)obj->widget;
	pstart();
	
	strcpy(widget->wname, "gfxbutton");
	widget->back_nor_name = NULL;
	widget->back_mon_name = NULL;
	widget->back_mdn_name = NULL;

	widget->st_mon = 0;
	widget->st_mdown = 0;

	widget->click = NULL;

	obj->destroyf = gfxbut_destroy;
	obj->load = gfxbut_load;
	obj->uload = gfxbut_uload;
	obj->enable = NULL;
	obj->disable = NULL;
	obj->hide = NULL;

	obj->dim = NULL;

	obj->set = gfxbut_set;
	obj->get = gfxbut_get;

	obj->mon = gfxbut_mon;
	obj->moff = gfxbut_moff;
	obj->mdown = gfxbut_mdown;
	obj->mup = gfxbut_mup;

	obj->kdown = NULL;
	obj->kup = NULL;

	obj->draw = gfxbut_draw;

	pstop();
	return KE_NONE;
}

kucode_t gfxbut_destroy( gui_obj_t *obj )
{
	gui_gfxbut_t *const widget = (gui_gfxbut_t*)obj->widget;
	pstart();

	if ( obj->status > GUI_NOTLOADED )
		gfxbut_uload(obj);

	if ( widget->back_nor_name )
		dfree(widget->back_nor_name);

	if ( widget->back_mon_name )
		dfree(widget->back_mon_name);

	if ( widget->back_mdn_name )
		dfree(widget->back_mdn_name);

	pstop();
	return KE_NONE;
}

kucode_t gfxbut_load( gui_obj_t *obj )
{
	gui_gfxbut_t *const widget = (gui_gfxbut_t*)obj->widget;
	pstart();

	widget->back_nor = res_access(widget->back_nor_name);
	if ( widget->back_nor == NULL )
	{
		plog(gettext("Object 'gfxbutton` %u background '%s` was not loaded: %d\n"), \
			obj->id, widget->back_nor_name, kucode);
		return kucode;
	}

	widget->back_mon = res_access(widget->back_mon_name);
	if ( widget->back_mon == NULL )
	{
		plog(gettext("Object 'gfxbutton` %u mouse-on background '%s` " \
			"was not loaded: %d\n"), \
			obj->id, widget->back_mon_name, kucode);
		res_release(widget->back_nor_name);
		return kucode;
	}

	widget->back_mdn = res_access(widget->back_mdn_name);
	if ( widget->back_mdn == NULL )
	{
		plog(gettext("Object 'gfxbutton` %u mouse-down background '%s` " \
			"was not loaded: %d\n"), \
			obj->id, widget->back_mdn_name, kucode);
		res_release(widget->back_mon_name);
		res_release(widget->back_nor_name);
		return kucode;
	}

	widget->face = widget->back_nor;

	pstop();
	return KE_NONE;
}

kucode_t gfxbut_uload( gui_obj_t *obj )
{
	gui_gfxbut_t *const widget = (gui_gfxbut_t*)obj->widget;
	pstart();

	if ( res_release(widget->back_nor_name) != KE_NONE )
		plog(gettext("Note: Object \"gfxbutton\" %u failed to release " \
			"a background \"%s\": %d\n"), obj->id, widget->back_nor_name, kucode);

	if ( res_release(widget->back_mon_name) != KE_NONE )
		plog(gettext("Note: Object \"gfxbutton\" %u failed to release " \
			"a mouse-on background \"%s\": %d\n"), obj->id, widget->back_mon_name, kucode);

	if ( res_release(widget->back_mdn_name) != KE_NONE )
		plog(gettext("Note: Object \"gfxbutton\" %u failed to release " \
			"a mouse-down background \"%s\": %d\n"), obj->id, widget->back_mdn_name, kucode);

	pstop();
	return KE_NONE;
}

kucode_t gfxbut_set( gui_obj_t *obj, int param, void *data )
{
	gui_gfxbut_t *const widget = (gui_gfxbut_t*)obj->widget;
	pstart();
	
	switch ( param )
	{
		case GFXBUT_NORM:
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
		case GFXBUT_MON:
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
		case GFXBUT_MDN:
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
		case GFXBUT_CLICK:
		{
			widget->click = (gui_cb_f)data;
			break;
		}
		default:
			KU_ERRQ(KE_INVALID);
	}

	pstop();
	return KE_NONE;
}

kucode_t gfxbut_get( gui_obj_t *obj, int param, void *data )
{
	gui_gfxbut_t *const widget = (gui_gfxbut_t*)obj->widget;
	pstart();

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

	pstop();
	return KE_NONE;
}

gui_event_st gfxbut_mon( gui_obj_t *obj, int x, int y, int z )
{
	gui_gfxbut_t *const widget = (gui_gfxbut_t*)obj->widget;
	pstart();

	widget->st_mon = 1;

	if ( !widget->st_mdown )
	{
		widget->face = widget->back_mon;
		if ( gfx_draw(widget->face, NULL, GFX_IMG_REAL, obj->rx, \
			obj->ry, 0, 0, 0, 0) != KE_NONE )
			return GUIE_ERROR;

		pstop();
		return GUIE_DRAW;
	}

	pstop();
	return GUIE_EAT;
}

gui_event_st gfxbut_moff( gui_obj_t *obj, int x, int y, int z )
{
	gui_gfxbut_t *const widget = (gui_gfxbut_t*)obj->widget;
	pstart();

	widget->st_mon = 0;

	if ( !widget->st_mdown )
	{
		widget->face = widget->back_nor;
		if ( gfx_draw(widget->face, NULL, GFX_IMG_REAL, obj->rx, obj->ry, \
			0, 0, 0, 0) != KE_NONE )
			return GUIE_ERROR;

		pstop();
		return GUIE_DRAW;
	}

	pstop();
	return GUIE_EAT;
}

gui_event_st gfxbut_mdown( gui_obj_t *obj, int x, int y, int z )
{
	gui_gfxbut_t *const widget = (gui_gfxbut_t*)obj->widget;
	pstart();

	widget->st_mdown = 1;

	widget->face = widget->back_mdn;
	if ( gfx_draw(widget->face, NULL, GFX_IMG_REAL, obj->rx, obj->ry, \
		0, 0, 0, 0) != KE_NONE )
		return GUIE_ERROR;

	pstop();
	return GUIE_DRAW;
}

gui_event_st gfxbut_mup( gui_obj_t *obj, int x, int y, int z )
{
	gui_gfxbut_t *const widget = (gui_gfxbut_t*)obj->widget;
	gui_event_st status;
	pstart();

	widget->st_mdown = 0;

	if ( widget->st_mon )
		status = gfxbut_mon(obj, x, y, z); else
		status = gfxbut_moff(obj, x, y, z);

	if ( widget->click && (widget->click(obj, (int)z) != KE_NONE) )
		return GUIE_ERROR;

	pstop();
	return status;
}

kucode_t gfxbut_draw( gui_obj_t *obj, int x, int y, int w, int h )
{
	gui_gfxbut_t *const widget = (gui_gfxbut_t*)obj->widget;
	pstart();

	if ( gfx_draw(widget->face, NULL, GFX_IMG_REAL, obj->rx, \
		obj->ry, x, y, w, h) != KE_NONE )
		return kucode;

	pstop();
	return KE_NONE;
}
