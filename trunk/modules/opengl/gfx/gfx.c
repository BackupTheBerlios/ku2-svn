/***************************************************************************
 *            gfx.c
 *
 *  Thu Jun  1 12:19:46 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include "SDL.h"
#include "SDL_ttf.h"

#if !defined(NO_NVIDIA_GL_H)
#include <nvidia/GL/gl.h>
#include <nvidia/GL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include "gfx.h"
#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/types.h"
#include "ku2/gettext.h"
#include "other/other.h"
#include "io/log/log.h"
#include "dp/resmanager/res.h"

kucode_t gfx_draw( gfx_image_t *src, int x, int y )
{
	GLenum format;
	pstart();
	
	glWindowPos2i(x, y);
	
	
	
	glDrawPixels(src->w, src->h, src->format, GL_UNSIGNED_BYTE, src->pixels);
	
	
	
	pstop();
	return KE_NONE;
}

SDL_Surface *gfx_txtrender ( const char *text, const gfx_font_t *font, SDL_Color colour )
{
	SDL_Surface *txtface;
	pstart();
	
	//TTF_SetFontStyle((TTF_Font*)font, style);
	//txtface = TTF_RenderUTF8_Blended((TTF_Font*)font, text, colour);
	if ( txtface == NULL )
	{
		plog(gettext("Error while rendering text: %s\n"), TTF_GetError());
		kucode = KE_EXTERNAL;
		return NULL;
	}
	
	pstop();
	return txtface;
}
