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
typedef
struct
{
	TTF_Font *font;	//!< Font data.
	char *name;		//!< Font name.
	gfx_font_style_t style;
					//!< Font style.
	int size;		//!< Font size.
	uint instances;	//!< Times this font is loaded.
}	gfx_font_t;

//! Initialize a font engine.
/*!
	Starts a font engine. This function assignes a font control
	function with type \a typeID.
	\param typeID Font type ID for resource manager.
	\retval KE_NONE No error.
	\retval KE_LIBRARY TTF_Init has failed.
	\retval KE_* res_assign() and abtree_create() errors.
	\sa font_halt().
*/
kucode_t font_init( int typeID );

//! Halt a font engine.
/*!
	Stops a font engine.
	\return Always \a KE_NONE.
	\sa font_init().
*/
kucode_t font_halt( void );

//! Open a font.
/*!
	Opens a font with the selected attributes.
	\param name Font name (for resource manager).
	\param style Font style.
	\param size Font size.
	\return Opened font or \e NULL if error, \ref kucode is set to the valid value: \n
	\a KE_MEMORY: Memory allocation has failed.\n
	res_access_adv() and abtree_ins() errors.
	\sa font_close() and font_change().
*/
gfx_font_t *font_open( const char *name, gfx_font_style_t style, int size );

//! Close a font.
/*!
	Closes a font.
	\param font Font to be closed.
	\return Always \a KE_NONE.
	\sa font_open() and font_change().
*/
kucode_t font_close( gfx_font_t *font );

//! Change font attributes.
/*!
	Changes font attributes.
	\param font Font to be changed.
	\param style New font style.
	\param size New font size.
	\return New font.
	\note This function is not realized yet!
	\sa font_open() and font_close().
*/
gfx_font_t *font_change( gfx_font_t *font, gfx_font_style_t style, int size );

#ifdef __cplusplus
}
#endif
#endif
