/***************************************************************************
 *            font.h
 *
 *  Sat Sep  9 14:39:31 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief SDL_ttf font management routines.

	Functions for font management operations.
	\author J. Anton
	\date Sat Sep 9 14:40:56 2006
	\version 1.1.0
*/

#ifndef KU__GFX_FONT_H__
#define KU__GFX_FONT_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "SDL_ttf.h"

#include "image.h"

#include "ku2/ecode.h"
#include "ku2/types.h"

//! Text style (bold, underlined or italics).
typedef
enum
{
	GFX_NORMAL = TTF_STYLE_NORMAL,
					//!< Normal.
	GFX_BOLD = TTF_STYLE_BOLD,
					//!< Bold.
	GFX_ITALIC = TTF_STYLE_ITALIC,
					//!< Italic.
	GFX_ULINED = TTF_STYLE_UNDERLINE
					//!< Underlined.
}	gfx_font_style_t;

//! Font object.
typedef TTF_Font gfx_font_t;

gfx_image_t *gfx_font_render( const char *text, gfx_font_t *font, gfx_font_style_t style, \
	uint8_t r, uint8_t g, uint8_t b );

//! Assign a font resource type and add resources from file.
/*!
	Assigns a font resource type (see \ref res.c) and adds resources from file.
	\param resource_type Resource ID.
	\param resfile File with list of resources.
	\retval KE_NONE No error.
	\return res_assign(), cfg_open(), cfg_query() and cfg_process() errors.
*/
kucode_t gfx_add_font_resources( int resource_type, const char *resfile );

#ifdef __cplusplus
}
#endif
#endif
