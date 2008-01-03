/***************************************************************************
 *            texted.h
 *
 *  Wed Aug 30 22:46:03 2006
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

#ifndef KU__GUI_TEXED_H__
#define KU__GUI_TEXED_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/image.h"
#include "gfx/font.h"
#include "gui/gui.h"
#include "ku2/ecode.h"

//! Text edit GUI object.
typedef
struct STRUCT_GUI_TEXED_OBJ
{
	char wname[6];
					//!< Widget name ('texed`).

	gui_cb_f
		keypress;	//!< Keypress callback function.

	char
		*back_nor_name,
					//!< Normal image name.
		*back_focus_name,
					//!< Focused background image name.
		*font_name;	//!< Font name.

	gfx_image_t
		*back_nor,	//!< Normal image.
		*back_focus;
					//!< Focus image.
	gfx_image_t
		*face,		//!< Current image surface.
		*fontface;	//!< Rendered text.

	gfx_font_t
		*font;		//!< Font.
	gfx_font_style_t
		font_style;	//!< Font style.
	uint8_t font_r;	//!< Font colour (red).
	uint8_t font_g;	//!< Font colour (green).
	uint8_t font_b;	//!< Font colour (blue).

	int textlen,	//!< Maximum length of the text.
		textpos;	//!< Current text length.
	char *text;		//!< Button caption.
}	gui_texed_t;

//! Text edit parameters for button_set() and button_get().
enum TEXED_PARAMS
{
	TEXED_NORM,		//!< Change the normal image. Set/Get: data are (char*/char**) image name (\ref res.h).
	TEXED_FOCUS,	//!< Change the "focus" image. Data are image name.
	TEXED_KP,		//!< Change the "key press" callback function. Data are pointer to function.
	TEXED_TEXTLEN,	//!< Change the text maximum length.
	TEXED_TEXT,		//!< Change the title of the button (button text).
	TEXED_FONT,		//!< Change the font.
	TEXED_FSTYLE,	//!< Change the font style.
	TEXED_FCR,		//!< Change the font colour (red).
	TEXED_FCG,		//!< Change the font colour (green).
	TEXED_FCB		//!< Change the font colour (blue).
};

//! Initialize a text edit object.
/*!
	This function should be passed to gui_obj_create() to create a text edit.
	It should set up all gui_obj_t variables to work correctly.
	\return Always \a KE_NONE.
	\sa gui_load_f(), button_destroy() and gui_obj_create().
*/
kucode_t texed_init( gui_obj_t *obj );

//! Destroy a button object.
/*!
	Frees all button related data and attributes.
	It is called in gui_obj_delete().
	\return Always \a KE_NONE.
	\sa gui_load_f(), button_init() and gui_obj_delete().
*/
kucode_t texed_destroy( gui_obj_t *obj );

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
kucode_t texed_load( gui_obj_t *obj );

//! Unload button related data.
/*!
	Unloads button related data.
	\return Always \a KE_NONE.
	\note All res_release() errors are logged if occured.
	\sa gui_status_f() and button_load().
*/
kucode_t texed_uload( gui_obj_t *obj );

kucode_t texed_update( gui_obj_t *obj );

//! Set the attribute of the button.
/*!
	Sets a button attribute.
	\retval KE_NONE No error.
	\retval KE_MEMORY Memory allocation has failed.
	\retval KE_INVALID Invalid attribute.
	\note If \a KE_MEMORY occured then old value is keeped unchanged.
	\sa gui_sg_f() and button_get().
*/
kucode_t texed_set( gui_obj_t *obj, int param, void *data );

//! Get the attribute of the button.
/*!
	Gets a button attribute.
	\retval KE_NONE No error.
	\retval KE_INVALID Invalid attribute.
	\sa gui_sg_f() and button_set().
*/
kucode_t texed_get( gui_obj_t *obj, int param, void *data );

//! Mouse-Down event handler.
/*!
	Handles "Mouse-Down" events.
	\return Always \a GUIE_EAT.
	\sa gui_mouse_f(), button_mon(), button_moff() and button_mup().
*/
gui_event_st texed_mdown( gui_obj_t *obj, int x, int y, int z );

gui_event_st texed_kdown( gui_obj_t *obj, char ch );
gui_event_st texed_defocus( gui_obj_t *obj );
//! Draw the button.
/*!
	Draws a button with the text if it is set.
	\retval KE_NONE No error.
	\retval KE_* gfx_draw() errors.
	\sa gui_draw_f().
*/
kucode_t texed_draw( gui_obj_t *obj, int x, int y );

#ifdef __cplusplus
}
#endif
#endif
