/***************************************************************************
 *            textline.h
 *
 *  Wed Aug 30 22:46:16 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Text line widget.

	Simple text on the screen.
	\author J. Anton
	\date Sat Sep 9 12:47:32 2006
	\version 1.0.0
*/

#ifndef KU__GUI_TEXT_H__
#define KU__GUI_TEXT_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/image.h"
#include "gfx/font.h"
#include "gui/gui.h"
#include "ku2/ecode.h"

//! Graphical button GUI object.
typedef
struct STRUCT_GUI_TEXT_OBJ
{
	char wname[5];	//!< Widget name. ('text')

	char
		*font_name;	//!< Font name.

	gfx_image_t *face;
	gfx_font_t *font;

	gfx_font_style_t
		font_style;	//!< Font style.
	uint8_t font_r;	//!< Font colour (red).
	uint8_t font_g;	//!< Font colour (green).
	uint8_t font_b;	//!< Font colour (blue).

	char *text;		//!< Text of an object.
}	gui_text_t;

//! Text parameters for text_set() and text_get().
enum TEXT_PARAMS
{
	TEXT_TEXT,
	TEXT_FONT,		//!< Change the font. Set/Get: data are (char*/char**) font name.
	TEXT_FSTYLE,
	TEXT_FCR,
	TEXT_FCG,
	TEXT_FCB
};

//! Initialize a graphical button object.
/*!
	This function should be passed to gui_obj_create() to create a graphical button.
	It should set up all gui_obj_t variables to work correctly.
	\return Always \a KE_NONE.
	\sa gui_load_f(), gfxbut_destroy() and gui_obj_create().
*/
kucode_t text_init( gui_obj_t *obj );

//! Destroy a graphical button object.
/*!
	Frees all button related data and attributes.
	It is called in gui_obj_delete().
	\return Always \a KE_NONE.
	\sa gui_load_f(), gfxbut_init() and gui_obj_delete().
*/
kucode_t text_destroy( gui_obj_t *obj );

//! Load a graphical button.
/*!
	Loads button data.
	\retval KE_NONE No error.
	\retval KE_* res_aceess() errors.
	\sa gui_status_f(), gfxbut_set() and gfxbut_uload().
*/
kucode_t text_load( gui_obj_t *obj );

//! Unload a graphical button.
/*!
	Unloads button data.
	\return Always \a KE_NONE.
	\sa gui_status_f() and gfxbut_load().
*/
kucode_t text_uload( gui_obj_t *obj );

kucode_t text_update( gui_obj_t *obj );

//kucode_t text_dim( gui_obj_t *obj );

//! Set the attribute of the graphical button.
/*!
	Sets a button attribute.
	\retval KE_NONE No error.
	\retval KE_MEMORY Memory allocation has failed.
	\retval KE_INVALID Invalid attribute.
	\sa gui_sg_f() and gfxbut_get().
*/
kucode_t text_set( gui_obj_t *obj, int param, void *data );

//! Get the attribute of the graphical button.
/*!
	Gets a button attribute.
	\retval KE_NONE No error.
	\retval KE_INVALID Invalid attribute.
	\sa gui_sg_f() and gfxbut_set().
*/
kucode_t text_get( gui_obj_t *obj, int param, void *data );

//! Draw the graphical button.
/*!
	Draws a button.
	\retval KE_NONE No error.
	\retval KE_* gfx_draw() errors.
	\sa gui_draw_f().
*/
kucode_t text_draw( gui_obj_t *obj, int x, int y );

#ifdef __cplusplus
}
#endif
#endif
