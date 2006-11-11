/***************************************************************************
 *            gfx.h
 *
 *  Thu Jun  1 12:18:51 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief SDL multimedia routines.
	
	Functions for basic multimedia operations.
	\author J. Anton
	\date Mon Aug 28 16:15:28 2006
	\version 1.1.0
*/

#ifndef KU__GFX_H__
#define KU__GFX_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "SDL.h"

#include "font.h"
#include "ku2/ecode.h"
#include "dp/resmanager/res.h"

//! Text alligment.
typedef
enum
{
	GFX_ALLIG_LEFT,	//!< By left side.
	GFX_ALLIG_MIDDLE,
					//!< By middle.
	GFX_ALLIG_RIGHT	//!< By right side.
}	gfx_txtallig_t;

//! Drawing mode.
typedef
enum
{
	//! Sprite mode.
	/*!
		Selected area of the source image will be drawn
		at \a x and \a y coordinates in the destination
		image.
	*/
	GFX_IMG_PIECE,
	
	//! Image mode.
	/*!
		Selected area of the source image will be drawn
		there, where it were if the whole source image
		was drawn at \a x and \a y coordinates in the
		destination image.
	*/
	GFX_IMG_REAL
}	gfx_imgmode_t;

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
kucode_t gfx_draw( SDL_Surface *src, int x, int y );

//! Render the text.
/*!
	Renders the text.
	\param text Text to be rendered.
	\param font Font to render with.
	\param colour Text colour.
	\return Rendered surface, or \e NULL if there was an error and
	\ref kucode is set to the valid value: \n
	\a KE_EXTERNAL: \b TTF_RenderUTF8_Blended() has failed.
*/
SDL_Surface *gfx_txtrender ( const char *text, const gfx_font_t *font, SDL_Color colour );

#ifdef __cplusplus
}
#endif
#endif
