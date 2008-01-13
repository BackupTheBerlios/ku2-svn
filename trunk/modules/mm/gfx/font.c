/*
		font.c
		Sun Nov 25 17:13:48 2007

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2007
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

#include <string.h>
#include <errno.h>

#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/gettext.h"
#include "dp/resmanager/res.h"
#include "io/cfgreader/cfg.h"
#include "io/log/log.h"

#include "font.h"
#include "image.h"

gfx_image_t *gfx_font_render( const char *text, gfx_font_t *font, \
							  gfx_font_style_t style, \
							  uint8_t r, uint8_t g, uint8_t b )
{
	gfx_image_t *img;
	SDL_Surface *sdlimg;
	SDL_Color colour;
	pstartp("text = %s, font = %p, style = %d, RGB = %u:%u:%u",
			text, font, style, r, g, b);
	
	colour.r = r;
	colour.g = g;
	colour.b = b;
	
	if ( (gfx_font_style_t)TTF_GetFontStyle(font) != style )
		TTF_SetFontStyle(font, style);
	sdlimg = TTF_RenderUTF8_Blended(font, text, colour);
	if ( sdlimg == NULL )
	{
		plogfn(gettext("Failed to renter a UTF8 text: %s.\n"), TTF_GetError());
		preturn NULL;
	}
	
	#if MM_BACKEND == SDL
	img = (gfx_image_t*)sdlimg;
	
	#elif MM_BACKEND == SDL_OGL
	img = gfx_img_fromSDL(sdlimg);
	if ( img == NULL )
	{
		SDL_FreeSurface(sdlimg);
		preturn NULL;
	}
	SDL_FreeSurface(sdlimg);
	#endif
	
	preturnp("img = %p", img) img;
}

static void *gfx_font_control( const char *path, rescontrol_t action, void *data )
{
	TTF_Font *font;
	pstartp("path = %s, action = %s, data = %p",
			path,
			(action == RES_LOAD) ? "load": "unload",
			data);
	
	if ( action == RES_LOAD )
	{
		font = TTF_OpenFont(path, (int)data);
		if ( font == NULL )
		{
			plog(gettext("Failed to open a font '%s`: %s\n"), path, TTF_GetError());
			preturn NULL;
		}
	}	else
	{
		TTF_CloseFont(data);
		preturn NULL;
	}
	
	preturnp("font = %p", font) font;
}

kucode_t gfx_add_font_resources( int resource_type, const char *resfile )
{
	uint i = 0;
	char font_id[CFG_BUFFER],
		font_file[CFG_BUFFER];
	int font_size;
	kucode_t code;
	cfg_session_t *cfg;
	pstart();

	if ( res_assign(resource_type, RESTYPE_ZFL, gfx_font_control) != KE_NONE )
	{
		plogfn(gettext("Failed to assign a font resource type: %d.\n"), KU_GET_ERROR());
		preturn KU_GET_ERROR();
	}
	
	if ( (cfg = cfg_open(resfile, CFG_STRICT|CFG_STEP)) == NULL )
	{
		if ( KU_GET_ERROR() == KE_IO )
			plogfn(gettext("Failed to open a file: %s.\n"), strerror(errno)); else
			plogfn(gettext("Failed to open a file: %d.\n"), KU_GET_ERROR());
		preturn KU_GET_ERROR();
	}
	
	if ( cfg_query(cfg, "font=ssi/r", font_id, font_file, &font_size) != KE_NONE )
	{
		plogfn(gettext("Failed to add a query for the resource file: %d.\n"),
			   KU_GET_ERROR());
		cfg_close(cfg);
		preturn KU_GET_ERROR();
	}
	
	while ( (code = cfg_process(cfg)) == KE_SIGNAL )
	{
		if ( res_add(font_file, font_id, resource_type, (void*)font_size, 0) != KE_NONE )
		{
			plogfn(gettext("Failed to add a font resource (line %d): %d.\n"), \
				cfg->cfg_line, KU_GET_ERROR());
			cfg_close(cfg);
			preturn KU_GET_ERROR();
		}
		i++;
	}
	if ( code != KE_NONE )
	{
		plogfn(gettext("Failed to process a resource file on line %d: %d.\n"), \
			cfg->cfg_line, code);
		cfg_close(cfg);
		preturn code;
	}
	
	if ( cfg_close(cfg) != KE_NONE )
		plogfn(gettext("Warning: failed to close a resource file: %d.\n"), KU_GET_ERROR());
	
	plogfn(gettext("%d fonts added.\n"), i);
	
	preturn KE_NONE;
}
