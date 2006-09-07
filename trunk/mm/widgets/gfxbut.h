/***************************************************************************
 *            gfxbut.h
 *
 *  Wed Aug 30 22:45:53 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Graphical button widget.

	Graphical button, no text. Thee images: normal state, "mouse-on" state and
	"mouse-down" state.
	\author J. Anton
	\date Thu Aug 31 21:35:22 2006
	\version 1.0.0
*/

#ifndef KU__GUI_GFXBUT_H__
#define KU__GUI_GFXBUT_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "SDL.h"
#include "gui/gui.h"
#include "ku2/ecode.h"

//! Graphical button GUI object.
typedef
struct STRUCT_GUI_GFXBUT_OBJ
{
	char wname[10];
					//!< Widget name.
	char
		*back_nor_name,
					//!< Normal image name.
		*back_mon_name,
					//!< "Mouse On" background image name.
		*back_mdn_name;
					//!< "Mouse Down" background image name.
	SDL_Surface
		*back_nor,	//!< Normal image.
		*back_mon,	//!< "Mouse On" image.
		*back_mdn;	//!< "Mouse Down" image.
	SDL_Surface
		*face;		//!< Current image surface.
}	gui_gfxbut_t;

//! Frame parameters for frame_set() and frame_get().
enum GFXBUT_PARAMS
{
	GFXBUT_NORM,	//!< Change the normal image. Data are image name (\ref res.h).
	GFXBUT_MON,		//!< Change the "mouse-on" image. Data are image name.
	GFXBUT_MDN,		//!< Change the "mouse-down" image. Data are image name.
	GFXBUT_CLICK	//!< Change the "click" callback function. Data are pointer to function.
};

//! Initialize a graphical button object.
/*!
	This function should be passed to gui_obj_create() to create a graphical button.
*/
kucode_t gfxbut_init( gui_obj_t *obj );

//! Load a graphical button.
kucode_t gfxbut_load( gui_obj_t *obj );

kucode_t gfxbut_uload( gui_obj_t *obj );

kucode_t gfxbut_enable( gui_obj_t *obj );
kucode_t gfxbut_dim( gui_obj_t *obj );

kucode_t gfxbut_set( gui_obj_t *obj, int param, void *data );

kucode_t gfxbut_get( gui_obj_t *obj, int param, void *data );

kucode_t gfxbut_draw( gui_obj_t *obj, int x, int y, int w, int h );

#ifdef __cplusplus
}
#endif
#endif
