/***************************************************************************
 *            gui_core.h
 *
 *  Fri Jun  2 17:39:42 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#ifndef __GUI_CORE_H__
#define __GUI_CORE_H__

#include "errors.h"
#include "SDL.h"

/*
	идентификатор объекта
*/
typedef
unsigned int gui_id;

/*
	типы объекта
*/
typedef
enum
{
	GUI_WINDOW,
	GUI_BUTTON,
	GUI_CONSTTEXT,
	GUI_EDITTEXT
}	gui_type_t;

/*
	параметры объекиа
*/
typedef
enum
{
	GUI_X,
	GUI_Y,
	GUI_WIDTH,
	GUI_HEIGHT,
	GUI_ENABLED,
	
	GUI_TEXT,
	GUI_TXT_ALLIG,
	
	GUI_FONT,
	GUI_FNT_COLOUR,
	GUI_FNT_STYLE,
	
	GUI_NEED_FRAME,
	GUI_IMAGE,
	GUI_IMG_DEACTIVE,
	GUI_IMG_MOUSEON,
	GUI_IMG_CLICK,
	
	GUI_SND_MOUSEON,
	GUI_SND_CLICK,
	
	GUI_CB_CLICK,
	GUI_CB_KEY
}	gui_field_t;

/*
	обработчик
*/
typedef
ecode_t (*gui_cb_f)( gui_id obj, void *data );

/*
	объект
*/
typedef struct GUI_OBJ_T gui_obj_t;

/*
	описание списка объектов
*/
#define LIST_TYPE gui_obj_t*
#define LIST_EQ(a, b) (a->id==b->id)
#define LIST_ERRVAL NULL
#define LIST_ID gui
#include "ku2/list/list_core.h"

/*
	функция инициализации типового объекта
*/
typedef
ecode_t (*gui_arg_f)( gui_obj_t *obj );
typedef
ecode_t (*gui_set_f)( gui_obj_t *obj, gui_field_t field, void *data );
typedef
ecode_t (*gui_draw_f)( gui_obj_t *obj, int x, int y, int w, int h );
typedef
ecode_t (*gui_event_f)( gui_obj_t *obj, SDL_Event *event, int rx, int ry );

/*
	описание объекта
*/
struct GUI_OBJ_T
{
	gui_id id;
	gui_type_t type;
	gui_obj_t *parent;
	kulist_t_gui *child;
	
	int x, y, width, height;
	int enabled, loaded;
	int rx, ry;
	
	gui_cb_f keypressed;
	gui_cb_f clicked;
	
	gui_arg_f arg_create, arg_clone, arg_delete;
	gui_set_f arg_set, arg_get;
	gui_arg_f arg_load, arg_uload;
	gui_draw_f arg_draw;
	
	gui_event_f mouse_on, mouse_off;
	gui_event_f mouse_down, mouse_up;
	gui_event_f key_down;
	
	void *arg;
};

ecode_t gui_init( void );
ecode_t gui_halt( void );

gui_id gui_obj_create( gui_type_t type );
gui_id gui_obj_clone( gui_id obj );
ecode_t gui_obj_delete( gui_id obj );

ecode_t gui_obj_set( gui_id obj, gui_field_t field, void *data );
ecode_t gui_obj_get( gui_id obj, gui_field_t field, void *data );

ecode_t gui_obj_load( gui_id obj );
ecode_t gui_obj_uload( gui_id obj );
ecode_t gui_obj_attach( gui_id obj, gui_id dest, int x, int y );
ecode_t gui_obj_release( gui_id obj );

ecode_t gui_set_root( gui_id obj );
ecode_t gui_set_active( gui_id obj );

ecode_t gui_draw( gui_obj_t *obj, int x, int y, int w, int h );
ecode_t gui_redraw( void );

ecode_t gui_manage_event( SDL_Event *event );

#endif
