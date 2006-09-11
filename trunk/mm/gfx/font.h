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

#include "SDL.h"
#include "SDL_ttf.h"

#include "ku2/ecode.h"
#include "resmanager/res.h"

//! Text style (bold, underlined or italics).
typedef
enum
{
	GFX_BOLD,
	GFX_ITALIC,
	GFX_ULINED
}	gfx_font_style_t;

typedef
struct
{
	TTF_Font *font;
	gfx_font_style_t style;
	int size;
}	gfx_font_t;

//! Font control function.
/*!
	Font control function for Resource manager.
	\note Parameter \a (\a int\a )data is loading font size.
	\sa rescontrol_f() and gfx_img_control().
*/
void *gfx_fnt_control( const char *path, rescontrol_t action, void *data );

gfx_font_t *font_open( const char *name, gfx_font_style_t style, int size );
kucode_t font_close( gfx_font_t *font );

gfx_font_t *font_change( gfx_font_t *font, gfx_font_style_t style, int size );

#ifdef __cplusplus
}
#endif
#endif
