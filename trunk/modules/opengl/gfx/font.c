/***************************************************************************
 *            font.c
 *
 *  Sun Sep 10 18:45:37 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <string.h>
#include <errno.h>

#include "font.h"
#include "image.h"

#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/gettext.h"

#include "dp/resmanager/res.h"
#include "io/cfgreader/cfg.h"
#include "io/log/log.h"

gfx_image_t *gfx_font_render( const char *text, gfx_font_t *font, gfx_font_style_t style, \
	uint8_t r, uint8_t g, uint8_t b )
{
	gfx_image_t *img;
	SDL_Surface *sdlimg;
	SDL_Color colour;
	pstart();

	colour.r = r;
	colour.g = g;
	colour.b = b;

	if ( TTF_GetFontStyle(font) != style )
		TTF_SetFontStyle(font, style);
	sdlimg = TTF_RenderUTF8_Blended(font, text, colour);
	if ( sdlimg == NULL )
	{
		plogfn(gettext("Failed to renter a UTF8 text: %s.\n"), TTF_GetError());
		return NULL;
	}

	img = gfx_img_fromSDL(sdlimg);
	if ( img == NULL )
	{
		SDL_FreeSurface(sdlimg);
		return NULL;
	}
	SDL_FreeSurface(sdlimg);

	pstop();
	return img;
}

static void *gfx_font_control( const char *path, rescontrol_t action, void *data )
{
	TTF_Font *font;
	pstart();

	if ( action == RES_LOAD )
	{
		font = TTF_OpenFont(path, (int)data);
		if ( font == NULL )
		{
			plog(gettext("Failed to open a font '%s`: %s\n"), path, TTF_GetError());
			return NULL;
		}
	}	else
	{
		TTF_CloseFont(data);
		return NULL;
	}

	pstop();
	return font;
}

kucode_t gfx_add_font_resources( int resource_type, const char *resfile )
{
	uint i = 0;
	char font_id[CFG_BUFFER],
		font_file[CFG_BUFFER];
	int font_size;
	kucode_t code;
	pstart();

	if ( res_assign(resource_type, RESTYPE_ZFL, gfx_font_control) != KE_NONE )
	{
		plogfn(gettext("Failed to assign a font resource type: %d.\n"), kucode);
		return kucode;
	}

	if ( cfg_open(resfile) != KE_NONE )
	{
		if ( kucode == KE_IO )
			plogfn(gettext("Failed to open a file: %s.\n"), strerror(errno)); else
			plogfn(gettext("Failed to open a file: %d.\n"), kucode);
		return kucode;
	}

	if ( cfg_query("font", "ssi", font_id, font_file, &font_size) != KE_NONE )
	{
		plogfn(gettext("Failed to add a query for the resource file: %d.\n"), kucode);
		cfg_close();
		return kucode;
	}

	while ( (code = cfg_process(CFG_STRICT|CFG_STEP)) == KE_SIGNAL )
	{
		if ( res_add(font_file, font_id, resource_type, (void*)font_size, 0) != KE_NONE )
		{
			plogfn(gettext("Failed to add a font resource (line %d): %d.\n"), \
				cfg_line, kucode);
			cfg_close();
			return kucode;
		}
		i++;
	}
	if ( code != KE_NONE )
	{
		plogfn(gettext("Failed to process a resource file on line %d: %d.\n"), \
			cfg_line, kucode);
		cfg_close();
		return kucode;
	}

	if ( cfg_close() != KE_NONE )
		plogfn(gettext("Warning: failed to close a resource file: %d.\n"), kucode);

	plogfn(gettext("%d fonts added.\n"), i);

	pstop();
	return KE_NONE;
}
