/***************************************************************************
 *            gfx.c
 *
 *  Thu Jun  1 12:19:46 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "gfx.h"
#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/types.h"
#include "ku2/gettext.h"
#include "other/other.h"
#include "log/log.h"
#include "resmanager/res.h"

SDL_Surface *screen;
int gfx_updated = 0;
gfx_resolution_t gfx_resolution;

void *gfx_img_control( const char *path, rescontrol_t action, void *data )
{
	SDL_Surface *img;
	pstart();

	if ( action == RES_LOAD )
	{
		img = IMG_Load(path);
		if ( img == NULL )
		{
			plog(gettext("Failed to load an image '%s`: %s\n"), path, IMG_GetError());
			return NULL;
		}
	}	else
	{
		SDL_FreeSurface(data);
		img = NULL;
	}

	pstop();
	return img;
}

void *gfx_fnt_control( const char *path, rescontrol_t action, void *data )
{
	TTF_Font *font;
	pstart();

	if ( action == RES_LOAD )
	{
		font = TTF_OpenFont(path, (int)data);
		if ( font == NULL )
		{
			plog(gettext("Failed to open a font '%s`: %s"), path, TTF_GetError());
			return NULL;
		}
	}	else
	{
		TTF_CloseFont(data);
		font = NULL;
	}

	pstop();
	return font;
}

kucode_t gfx_create_window( int width, int height, int bpp, int fullscreen,
						   gfx_mode_t mode, const char *caption, const char *bar_caption )
{
	uint32_t flags = 0;
	const SDL_VideoInfo *vi;
	char dname[64];
	pstart();
	
	if ( (width >= 800) && (height >= 600) )
		gfx_resolution = GFX_RES_640; else
	if ( (width >= 640) && (height >= 480) )
		gfx_resolution = GFX_RES_640; else
	{
		plog(gettext("Invalid resolution: %dx%dx%d.\n"), width, height, bpp);
		KU_ERRQ(KE_INVALID);
	}
	
	vi = SDL_GetVideoInfo();
	
	switch ( mode )
	{
		case GFX_SOFTWARE:
		{
			flags |= SDL_SWSURFACE;
			break;
		}
		case GFX_HARDWARE:
		{
			flags |= SDL_HWSURFACE;
			flags |= SDL_DOUBLEBUF;
			break;
		}
	}
	
	if ( fullscreen )
		flags |= SDL_FULLSCREEN;
	
	screen = SDL_SetVideoMode(width, height, bpp, flags);
	if ( screen == NULL )
	{
		plog(gettext("Failed to set the video mode: %s\n"), SDL_GetError());
		KU_ERRQ(KE_EXTERNAL);
	}
	
	SDL_WM_SetCaption(caption, bar_caption);
	
	SDL_VideoDriverName(dname, 64);
	plog(gettext("Video mode has been set.\n\tDriver: %s\n" \
		"\tHardware acceleration: %d\n" \
		"\tWindow manager: %d\n" \
		"\tBlit availability: %d %d %d %d %d %d %d\n" \
		"\tVideo memory: %d\n"),
		dname,
		vi->hw_available, vi->wm_available,
		vi->blit_hw, vi->blit_hw_CC, vi->blit_hw_A,
		vi->blit_sw, vi->blit_sw_CC, vi->blit_sw_A,
		vi->blit_fill, vi->video_mem);
	
	pstop();
	return KE_NONE;
}

kucode_t gfx_draw( const SDL_Surface *src, SDL_Surface *dst, gfx_imgmode_t mode, \
				  int x, int y, int _x, int _y, int _w, int _h )
{
	SDL_Rect rs, rd;
	pstart();
	
	if ( dst == NULL )
		dst = screen;

	rs.x = _x;
	rs.y = _y;
	rs.w = _w;
	rs.h = _h;
	if ( rs.w == 0 ) rs.w = src->w;
	if ( rs.h == 0 ) rs.h = src->h;
	
	if ( mode == GFX_IMG_PIECE )
	{
		rd.x = x;
		rd.y = y;
	}	else
	{
		rd.x = x+rs.x;
		rd.y = y+rs.y;
	}
	
	if ( SDL_BlitSurface((SDL_Surface*)src, &rs, dst, &rd) != 0 )
	{
		plog(gettext("Failed to draw an image: %s\n"), SDL_GetError());
		KU_ERRQ(KE_EXTERNAL);
	}
	
	gfx_updated = 1;
	
	pstop();
	return KE_NONE;
}

#if 0
SDL_Surface *gfx_txtrender ( const char *text, const TTF_Font *font, gfx_txtstyle_t style, SDL_Color colour )
{
	SDL_Surface *txtface;
	pstart();
	
	TTF_SetFontStyle((TTF_Font*)font,style);
	txtface=TTF_RenderUTF8_Blended((TTF_Font*)font,text,colour);
	if ( txtface==NULL )
	{
		SET_ERROR(E_SDL,vstr(gettext("Error while rendering text: %s"),TTF_GetError()));
		return NULL;
	}
	
	pstop();
	return txtface;
}

ecode_t gfx_print( const char *text, const TTF_Font *font, SDL_Surface *dst, gfx_txtstyle_t style, \
		SDL_Color colour, int x, int y, int _x, int _y, int _w, int _h )
{
	SDL_Surface *txtface;
	pstart();
	
	txtface=gfx_txtrender(text,font,style,colour);
	if ( txtface==NULL )
		return ecode;
	
	if ( gfx_draw(txtface,dst,GFX_IMG_PIECE,x,y,_x,_y,_w,_h)!=E_NONE )
	{
		SDL_FreeSurface(txtface);
		return ecode;
	}
	SDL_FreeSurface(txtface);
	
	pstop();
	return E_NONE;
}

ecode_t gfx_printf( const char *text, const TTF_Font *font, SDL_Surface *dst, gfx_txtstyle_t style, \
		SDL_Color colour, int x, int y, int w, int h, gfx_txtallig_t allig )
{
	pstart();
	pstop();
	return E_NONE;
}
#endif
