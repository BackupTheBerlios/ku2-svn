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

#include "SDL.h"
#include "gui/gui.h"
#include "ku2/ecode.h"

//! Frame GUI object.
typedef
struct STRUCT_GUI_FRAME_OBJ
{
	char wname[6];
					//!< Widget name.
	char *background_name;
					//!< Background image name.
	SDL_Surface *background;
					//!< Backgdound image surface.
}	gui_frame_t;

//! Frame parameters for frame_set() and frame_get().
enum FRAME_PARAMS
{
	FRAME_BACKGROUND
					//!< Change the background. Data are image name (\ref res.h).
};

//! Initialize a frame object.
/*!
	This function should be passed to gui_obj_create() to create a frame.
*/
kucode_t frame_init( gui_obj_t *obj );

//! Load a frame.
kucode_t frame_load( gui_obj_t *obj );

//! Unload a frame.
kucode_t frame_uload( gui_obj_t *obj );

kucode_t frame_enable( gui_obj_t *obj );
kucode_t frame_dim( gui_obj_t *obj );

//! Set the attribute of the frame.
kucode_t frame_set( gui_obj_t *obj, int param, void *data );

//! Get the attribute of the frame.
kucode_t frame_get( gui_obj_t *obj, int param, void *data );

//! Draw the frame.
kucode_t frame_draw( gui_obj_t *obj, int x, int y, int w, int h );

#ifdef __cplusplus
}
#endif
#endif
