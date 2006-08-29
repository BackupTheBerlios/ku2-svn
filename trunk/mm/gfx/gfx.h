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
#include "SDL_ttf.h"

#include "ku2/ecode.h"
#include "resmanager/res.h"

//! Screen mode.
typedef
enum
{
	GFX_SOFTWARE,	//!< Software screen.
	GFX_HARDWARE	//!< Hardware screen.
}	gfx_mode_t;

//! Text alligment.
typedef
enum
{
	GFX_ALLIG_LEFT,	//!< By left side.
	GFX_ALLIG_MIDDLE,
					//!< By middle.
	GFX_ALLIG_RIGHT	//!< By right side.
}	gfx_txtallig_t;

//! Text style (bold, underlined or italics).
typedef
int gfx_txtstyle_t;

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

//! The most similar standart video mode.
typedef
enum
{
	GFX_RES_640,	//!< Video mode 640x480
	GFX_RES_800		//!< Video mode 800x600
}	gfx_resolution_t;

//! Screen surface.
extern SDL_Surface *screen;

//! The most similar standart mode.
extern gfx_resolution_t gfx_resolution;

//! Image control function.
/*!
	Image control function for Resource manager.
	\note Parameter \a data is not used.
	\sa rescontrol_f() and gfx_fnt_control().
*/
void *gfx_img_control( const char *path, rescontrol_t action, void *data );

//! Font control function.
/*!
	Font control function for Resource manager.
	\note Parameter \a (\a int\a )data is loading font size.
	\sa rescontrol_f() and gfx_img_control().
*/
void *gfx_fnt_control( const char *path, rescontrol_t action, void *data );

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
	\note If \a _w is zero then the while width is copied. \n
	If \a _h is zero then the whole height is copied. \n
	if \a dst is \e NULL then the source image is copied on the screen.
*/
kucode_t gfx_draw( const SDL_Surface *src, SDL_Surface *dst, gfx_imgmode_t mode, \
				  int x, int y, int _x, int _y, int _w, int _h );

//! Render the text.
/*!
	Renders the text.
	\param text Text to be rendered.
	\param font Font to render with.
	\param style Font style (see SDL_ttf.h).
	\param colour Text colour.
	\return Rendered surface, or \e NULL if there was an error and
	\ref kucode is set to the valid value: \n
	\a KE_EXTERNAL: \b TTF_RenderUTF8_Blended() has failed.
*/
SDL_Surface *gfx_txtrender ( const char *text, const TTF_Font *font, gfx_txtstyle_t style, SDL_Color colour );

/*
	1.	Написать текст на изображении
	2.	text - текст
		font - шрифт
		dst - изображение назначения
		style - стиль шрифта
		colour - цвет шрифта
		x, y - координаты назначения
		_x, _y, _w, _h - участок исходного текста
	3.	---
	4.	ошибки gfx_txtrender и gfx_draw
*/
kucode_t gfx_print( const char *text, const TTF_Font *font, SDL_Surface *dst, gfx_txtstyle_t style, \
		SDL_Color colour, int x, int y, int _x, int _y, int _w, int _h );

/*
	NOT IMPLEMENTED
*/
kucode_t gfx_printf( const char *text, const TTF_Font *font, SDL_Surface *dst, gfx_txtstyle_t style, \
		SDL_Color colour, int x, int y, int w, int h, gfx_txtallig_t allig );

#ifdef __cplusplus
}
#endif
#endif
