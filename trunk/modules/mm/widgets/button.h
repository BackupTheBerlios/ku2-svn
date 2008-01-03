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

	Button with the text. Thee images: normal state, "mouse-on" state and
	"mouse-down" state. Font, font colour and caption.
	\author J. Anton
	\date Sun Dec 10 2006
	\version 1.1.0
*/

#ifndef KU__GUI_BUTTON_H__
#define KU__GUI_BUTTON_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/image.h"
#include "gfx/font.h"
#include "gui/gui.h"
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

	int font_rx,	//!< Relative caption X.
		font_ry;	//!< Relative caption Y.

	gfx_font_t
		*font;		//!< Font.
	gfx_font_style_t
		font_style;	//!< Font style.
	uint8_t font_r;	//!< Font colour (red).
	uint8_t font_g;	//!< Font colour (green).
	uint8_t font_b;	//!< Font colour (blue).

	char *caption;	//!< Button caption.
}	gui_button_t;

//! Button parameters for button_set() and button_get().
enum BUTTON_PARAMS
{
	BUTTON_NORM,	//!< Change the normal image. Set/Get: data are (char*/char**) image name (\ref res.h).
	BUTTON_MON,		//!< Change the "mouse-on" image. Data are image name.
	BUTTON_MDN,		//!< Change the "mouse-down" image. Data are image name.
	BUTTON_CLICK,	//!< Change the "click" callback function. Data are pointer to function.
	BUTTON_CAPTION,	//!< Change the title of the button (button text).
	BUTTON_FONT,	//!< Change the font.
	BUTTON_FSTYLE,	//!< Change the font style.
	BUTTON_FCR,		//!< Change the font colour (red).
	BUTTON_FCG,		//!< Change the font colour (green).
	BUTTON_FCB		//!< Change the font colour (blue).
};

//! Initialize a button object.
/*!
	This function should be passed to gui_obj_create() to create a button.
	It should set up all gui_obj_t variables to work correctly.
	\return Always \a KE_NONE.
	\sa gui_load_f(), button_destroy() and gui_obj_create().
*/
kucode_t button_init( gui_obj_t *obj );

//! Destroy a button object.
/*!
	Frees all button related data and attributes.
	It is called in gui_obj_delete().
	\return Always \a KE_NONE.
	\sa gui_load_f(), button_init() and gui_obj_delete().
*/
kucode_t button_destroy( gui_obj_t *obj );

//! Load button related data.
/*!
	Loads button related data data (images, fonts).
	\ref BUTTON_NORM should be set previously. Other settings are optional, if
	they are not set then additional feachures are diabled.
	\retval KE_NONE No error.
	\retval KE_* res_aceess() errors.
	\note Only BUTTON_NORM errors are returned, others are not critical and will only be logged.
	\sa gui_status_f(), button_set() and button_uload().
*/
kucode_t button_load( gui_obj_t *obj );

//! Unload button related data.
/*!
	Unloads button related data.
	\return Always \a KE_NONE.
	\note All res_release() errors are logged if occured.
	\sa gui_status_f() and button_load().
*/
kucode_t button_uload( gui_obj_t *obj );

kucode_t button_update( gui_obj_t *obj );

//! Set the attribute of the button.
/*!
	Sets a button attribute.
	\retval KE_NONE No error.
	\retval KE_MEMORY Memory allocation has failed.
	\retval KE_INVALID Invalid attribute.
	\note If \a KE_MEMORY occured then old value is keeped unchanged.
	\sa gui_sg_f() and button_get().
*/
kucode_t button_set( gui_obj_t *obj, int param, void *data );

//! Get the attribute of the button.
/*!
	Gets a button attribute.
	\retval KE_NONE No error.
	\retval KE_INVALID Invalid attribute.
	\sa gui_sg_f() and button_set().
*/
kucode_t button_get( gui_obj_t *obj, int param, void *data );

//! Mouse-On event handler.
/*!
	Handles "Mouse-On" events.
	\return Always \a GUIE_EAT.
	\sa gui_mouse_f(), button_moff(), button_mdown() and button_mup().
*/
gui_event_st button_mon( gui_obj_t *obj, int x, int y, int z );

//! Mouse-Off event handler.
/*!
	Handles "Mouse-Off" events.
	\return Always \a GUIE_EAT.
	\sa gui_mouse_f(), button_mon(), button_mdown() and button_mup().
*/
gui_event_st button_moff( gui_obj_t *obj, int x, int y, int z );

//! Mouse-Down event handler.
/*!
	Handles "Mouse-Down" events.
	\return Always \a GUIE_EAT.
	\sa gui_mouse_f(), button_mon(), button_moff() and button_mup().
*/
gui_event_st button_mdown( gui_obj_t *obj, int x, int y, int z );

//! Mouse-Up event handler.
/*!
	Handles 'Mouse-Up` events.
	\retval GUIE_EAT Everything is OK.
	\retval GUIE_ERROR Click callback function has failed.
	\sa gui_mouse_f(), gui_cb_f(), button_mon(), button_moff() and button_mdown().
*/
gui_event_st button_mup( gui_obj_t *obj, int x, int y, int z );

//! Draw the button.
/*!
	Draws a button with the text if it is set.
	\retval KE_NONE No error.
	\retval KE_* gfx_draw() errors.
	\sa gui_draw_f().
*/
kucode_t button_draw( gui_obj_t *obj, int x, int y );

#ifdef __cplusplus
}
#endif
#endif
