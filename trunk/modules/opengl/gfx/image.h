/*
		image.h
		Sun Nov 25 16:57:17 2007

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2007
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

/*!
	\file
	\brief Image control.

	Functions for image control.
	\author J. Anton
*/

#ifndef KU__IMAGE_H__
#define KU__IMAGE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "SDL.h"

#include "ku2/ecode.h"
#include "ku2/types.h"

//! Image.
typedef
struct
{
	uint32_t format;
					//!< GL pixel format.
	uint w,			//!< Image width.
		h;			//!< Image height.
	void *pixels;	//!< Pixel data.
}	gfx_image_t;

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
kucode_t gfx_draw( gfx_image_t *src, int x, int y );

gfx_image_t *gfx_img_fromSDL( SDL_Surface *src );

//! Assign an image resource type and add resources from file.
/*!
	Assigns an image resource type (see \ref res.c) and adds resources from file.
	\param resource_type Resource ID.
	\param resfile File with list of resources.
	\retval KE_NONE No error.
	\return res_assign(), cfg_open(), cfg_query() and cfg_process() errors.
*/
kucode_t gfx_add_image_resources( int resource_type, const char *resfile );

#ifdef __cplusplus
}
#endif
#endif
