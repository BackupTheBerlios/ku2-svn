/***************************************************************************
 *            button.h
 *
 *  Wed Aug 30 22:45:53 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Button widget.

	Graphical button, no text. Thee images: normal state, "mouse-on" state and
	"mouse-down" state.
	\author J. Anton
	\date Thu Aug 31 21:35:22 2006
	\version 1.0.0
*/

#ifndef KU__GUI_BUTTON_H__
#define KU__GUI_BUTTON_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "modules/opengl/gfx/image.h"
#include "modules/opengl/gui/gui.h"
#include "ku2/ecode.h"

//! Button GUI object.
typedef
struct STRUCT_GUI_BUTTON_OBJ
{
	char wname[7];
					//!< Widget name ('button`).

	int st_mon,		//!< Whether mouse is upon the widget.
		st_mdown;	//!< Whether mouse button is pressed.

	gui_cb_f
		click;		//!< Click callback function.

	char
		*back_nor_name,
					//!< Normal image name.
		*back_mon_name,
					//!< "Mouse On" background image name.
		*back_mdn_name,
					//!< "Mouse Down" background image name.
		*font_name;	//!< Font name.

	gfx_image_t
		*back_nor,	//!< Normal image.
		*back_mon,	//!< "Mouse On" image.
		*back_mdn;	//!< "Mouse Down" image.
	gfx_image_t
		*face,		//!< Current image surface.
		*fontface;	//!< Rendered text.

	gfx_font_t
		*font;		//!< Font.
	gfx_font_style_t
		font_style;	//!< Font style.

	char *caption;	//!< Button caption.
}	gui_button_t;

//! Button parameters for gfxbut_set() and gfxbut_get().
enum BUTTON_PARAMS
{
	BUTTON_NORM,	//!< Change the normal image. Set/Get: data are (char*/char**) image name (\ref res.h).
	BUTTON_MON,		//!< Change the "mouse-on" image. Data are image name.
	BUTTON_MDN,		//!< Change the "mouse-down" image. Data are image name.
	BUTTON_CLICK,	//!< Change the "click" callback function. Data are pointer to function.
	BUTTON_CAPTION,	//!< Change the title of the button (button text).
	BUTTON_FONT,	//!< Change the font.
	BUTTON_FSTYLE
};

//! Initialize a graphical button object.
/*!
	This function should be passed to gui_obj_create() to create a graphical button.
	It should set up all gui_obj_t variables to work correctly.
	\return Always \a KE_NONE.
	\sa gui_load_f(), gfxbut_destroy() and gui_obj_create().
*/
kucode_t button_init( gui_obj_t *obj );

//! Destroy a graphical button object.
/*!
	Frees all button related data and attributes.
	It is called in gui_obj_delete().
	\return Always \a KE_NONE.
	\sa gui_load_f(), gfxbut_init() and gui_obj_delete().
*/
kucode_t button_destroy( gui_obj_t *obj );

//! Load a graphical button.
/*!
	Loads button data.
	\retval KE_NONE No error.
	\retval KE_* res_aceess() errors.
	\sa gui_status_f(), gfxbut_set() and gfxbut_uload().
*/
kucode_t button_load( gui_obj_t *obj );

//! Unload a graphical button.
/*!
	Unloads button data.
	\return Always \a KE_NONE.
	\sa gui_status_f() and gfxbut_load().
*/
kucode_t button_uload( gui_obj_t *obj );

//! Change the button dimentions
/*!
	Changes the button dimentions to normal image dimentions.
	\return Always \a KE_NONE.
	\sa gui_dim_f().
*/
kucode_t button_dim( gui_obj_t *obj );

//! Set the attribute of the graphical button.
/*!
	Sets a button attribute.
	\retval KE_NONE No error.
	\retval KE_MEMORY Memory allocation has failed.
	\retval KE_INVALID Invalid attribute.
	\sa gui_sg_f() and gfxbut_get().
*/
kucode_t button_set( gui_obj_t *obj, int param, void *data );

//! Get the attribute of the graphical button.
/*!
	Gets a button attribute.
	\retval KE_NONE No error.
	\retval KE_INVALID Invalid attribute.
	\sa gui_sg_f() and gfxbut_set().
*/
kucode_t button_get( gui_obj_t *obj, int param, void *data );

//! Mouse-On event handler.
/*!
	Handles "Mouse-On" events.
	\return Possible \a GUIE_ERROR with \ref kucode set to gfx_draw() errors.
	\sa gui_mouse_f(), gfxbut_moff(), gfxbut_mdown() and gfxbut_mup().
*/
gui_event_st button_mon( gui_obj_t *obj, int x, int y, int z );

//! Mouse-Off event handler.
/*!
	Handles "Mouse-Off" events.
	\return Possible \a GUIE_ERROR with \ref kucode set to gfx_draw() errors.
	\sa gui_mouse_f(), gfxbut_mon(), gfxbut_mdown() and gfxbut_mup().
*/
gui_event_st button_moff( gui_obj_t *obj, int x, int y, int z );

//! Mouse-Down event handler.
/*!
	Handles "Mouse-Down" events.
	\return Possible \a GUIE_ERROR with \ref kucode set to gfx_draw() errors.
	\sa gui_mouse_f(), gfxbut_mon(), gfxbut_moff() and gfxbut_mup().
*/
gui_event_st button_mdown( gui_obj_t *obj, int x, int y, int z );

//! Mouse-Up event handler.
/*!
	Handles 'Mouse-Up` events.
	\return The same as gfxbut_mon() and gfxbut_moff().
	\sa gui_mouse_f(), gfxbut_mon(), gfxbut_moff() and gfxbut_mdown().
*/
gui_event_st button_mup( gui_obj_t *obj, int x, int y, int z );

//! Draw the graphical button.
/*!
	Draws a button.
	\retval KE_NONE No error.
	\retval KE_* gfx_draw() errors.
	\sa gui_draw_f().
*/
kucode_t button_draw( gui_obj_t *obj, int x, int y );

#ifdef __cplusplus
}
#endif
#endif
