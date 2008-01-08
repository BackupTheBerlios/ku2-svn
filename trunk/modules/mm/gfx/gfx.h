/*
		gfx.h
		Tue Dec 25 17:16:35 2007

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2007
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

/*!
	\file
	\brief SDL multimedia routines.
	
	Functions for basic multimedia operations.
	\author J. Anton
*/
 
#ifndef KU__GFX_H__
#define KU__GFX_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "SDL.h"
#include "ku2/ecode.h"

//! Screen mode.
typedef
enum
{
	GFX_SOFTWARE,	//!< Software SDL 2D screen.
	GFX_HARDWARE,	//!< Hardware SDL 2D screen.
	GFX_OPENGL		//!< Hardware OpenGL screen.
}	gfx_mode_t;

//! Text alligment.
typedef
enum
{
	GFX_ALLIG_LEFT, //!< By left side.
GFX_ALLIG_MIDDLE,
//!< By middle.
GFX_ALLIG_RIGHT //!< By right side.
} gfx_txtallig_t;
 

 
//! The most similar standart video mode.
typedef
enum
{
	GFX_RES_640, //!< Video mode 640x480
	GFX_RES_800 //!< Video mode 800x600
}	gfx_resolution_t;
 
//! Screen surface.
extern SDL_Surface *gfx_screen;
 
//! The most similar standart mode.
extern gfx_resolution_t gfx_resolution;

extern int gfx_needs_update;

//! Set a video mode and create a window.
/*!
Sets a video mode and, if needed, creates a window.
\param width Screen width.
\param height Screen height.
\param bpp Bits per pixel (colour mode).
\param fullscreen Fulscreen mode.
\param mode Video adapter mode.
\param caption Caption of the window.
\param bar_caption Caption of the window in window list.
\retval KE_NONE No error.
\retval KE_INVALID Invalid resolution.
\retval KE_EXTERNAL \b SDL_SetVideoMode() has failed.
\note Window will be doestroyed only after \b SDL_Quit() call.
*/
kucode_t gfx_create_window( int width, int height, int bpp, int fullscreen, \
						    gfx_mode_t mode, const char *caption, const char *bar_caption );

void gfx_screen_update( void );

//! Draw an image.
/*!
Draws and image \a src on \a dst.
\param src Source image.
\param dst Image, where to draw the source image.
\param mode Drawing mode.
\param x Destination \a x.
\param y Destination \a y.
\param _x Source \a x.
\param _y Source \a y.
\param _w Source image width.
\param _h Source image height.
\retval KE_NONE No error.
\retval KE_EXTERNAL \b SDL_BlitSurface() has failed.
\note If \a _w is zero then the whole width is copied. \n
If \a _h is zero then the whole height is copied. \n
if \a dst is \e NULL then the source image is copied on the screen.
*/

#ifdef __cplusplus
}
#endif
#endif
