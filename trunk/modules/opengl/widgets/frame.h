/***************************************************************************
 *            frame.h
 *
 *  Wed Aug 30 22:45:35 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Frame (static window) widget.

	Static area, which can include other objects. Has a constant background.
	\author J. Anton
	\date Thu Aug 31 10:45:33 2006
	\version 1.0.0
*/

#ifndef KU__GUI_FRAME_H__
#define KU__GUI_FRAME_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/image.h"
#include "gui/gui.h"
#include "ku2/ecode.h"

//! Frame GUI object.
typedef
struct STRUCT_GUI_FRAME_OBJ
{
	char wname[6];
					//!< Widget name ('frame`).
	char *background_name;
					//!< Background image name.
	gfx_image_t *background;
					//!< Backgdound image surface.
}	gui_frame_t;

//! Frame parameters for frame_set() and frame_get().
enum FRAME_PARAMS
{
	FRAME_BACKGROUND
					//!< Change the background. Set/Get: data are 'char*`/'char**` image name (\ref res.h).
};

//! Initialize a frame object.
/*!
	This function should be passed to gui_obj_create() to create a frame.
	It should set up all gui_obj_t variables to work correctly.
	\return Always \a KE_NONE.
	\sa gui_load_f(), frame_destroy() and gui_obj_create().
*/
kucode_t frame_init( gui_obj_t *obj );

//! Destroy a frame object.
/*!
	Frees all frame related data and attributes.
	It is called in gui_obj_delete().
	\return Always \a KE_NONE.
	\sa gui_load_f(), frame_init() and gui_obj_delete().
*/
kucode_t frame_destroy( gui_obj_t *obj );

//! Load a frame.
/*!
	Loads frame data.
	\retval KE_NONE No error.
	\retval KE_* res_aceess() errors.
	\sa gui_status_f(), frame_set() and frame_uload().
*/
kucode_t frame_load( gui_obj_t *obj );

//! Unload a frame.
/*!
	Unloads frame data.
	\return Always \a KE_NONE.
	\sa gui_status_f() and frame_load().
*/
kucode_t frame_uload( gui_obj_t *obj );

//! Set the attribute of the frame.
/*!
	Sets a frame attribute.
	\retval KE_NONE No error.
	\retval KE_MEMORY Memory allocation has failed.
	\retval KE_INVALID Invalid attribute.
	\sa gui_sg_f() and frame_get().
*/
kucode_t frame_set( gui_obj_t *obj, int param, void *data );

//! Get the attribute of the frame.
/*!
	Gets a frame attribute.
	\retval KE_NONE No error.
	\retval KE_INVALID Invalid attribute.
	\sa gui_sg_f() and frame_set().
*/
kucode_t frame_get( gui_obj_t *obj, int param, void *data );

//! Draw the frame.
/*!
	Draws a frame.
	\retval KE_NONE No error.
	\retval KE_* gfx_draw() errors.
	\sa gui_draw_f().
*/
kucode_t frame_draw( gui_obj_t *obj, int x, int y );

#ifdef __cplusplus
}
#endif
#endif
