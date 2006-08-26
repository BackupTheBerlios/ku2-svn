/***************************************************************************
 *            gfx.c
 *
 *  Thu Jun  1 12:19:46 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <libintl.h>

#include "gfx.h"
#include "evoley.h"
#include "ku2/errors/debug.h"
#include "ku2/other/other.h"
#include "ku2/log/log.h"

#include "SDL_image.h"
#include "SDL_ttf.h"

SDL_Surface *screen;
int gfx_updated=0;
gfx_resolution_t gfx_resolution;

ecode_t gfx_create_window( int width, int height, int bpp, int fullscreen, gfx_mode_t mode )
{
	Uint32 flags=0;
	const SDL_VideoInfo *vi;
	char dname[64];
	pstart();
	
	if ( (width>=800) && (height>=600) )
		gfx_resolution=GFX_RES_640; else
	if ( (width>=640) && (height>=480) )
		gfx_resolution=GFX_RES_640; else
		{
			SET_ERROR(E_INVALID,gettext("Invalid resolution."));
			return E_INVALID;
		}
	
	vi=SDL_GetVideoInfo();
	
	switch ( mode )
	{
		case GFX_SOFTWARE:
		{
			flags|=SDL_SWSURFACE;
			break;
		}
		case GFX_HARDWARE:
		{
			flags|=SDL_HWSURFACE;
			flags|=SDL_DOUBLEBUF;
			break;
		}
	}
	
	if ( fullscreen )
		flags|=SDL_FULLSCREEN;
	
	screen=SDL_SetVideoMode(width,height,bpp,flags);
	if ( screen==NULL )
	{
		SET_ERROR(E_SDL,qstr("Failed to set the video mode: ",SDL_GetError()));
		return E_SDL;
	}
	
	SDL_WM_SetCaption(STR(CAPTION)" version "STR(VERSION),STR(CAPTION));
	
	SDL_VideoDriverName(dname,64);
	plog(gettext("Video mode has been set.\n\tDriver: %s\n" \
		"\tHardware acceleration: %d\n" \
		"\tWindow manager: %d\n" \
		"\tBlit availability: %d %d %d %d %d %d %d\n" \
		"\tVideo memory: %d\n"),
		dname,
		vi->hw_available,vi->wm_available,
		vi->blit_hw,vi->blit_hw_CC,vi->blit_hw_A,
		vi->blit_sw,vi->blit_sw_CC,vi->blit_sw_A,
		vi->blit_fill,vi->video_mem);
	
	pstop();
	return E_NONE;
}

void *gfx_img_control( const char *path, rescontrol_t mode, void *data )
{
	SDL_Surface *img=NULL;
	pstart();
	if ( mode==RES_LOAD )
	{
		img=IMG_Load(path);
		if ( img==NULL )
		{
			SET_ERROR(E_SDL,vstr(gettext("Failed to load an image '%s`: %s"),path,IMG_GetError()));
			return NULL;
		}
	}	else
	{
		SDL_FreeSurface(data);
	}
	pstop();
	return img;
}

void *gfx_fnt_control( const char *path, rescontrol_t mode, void *data )
{
	TTF_Font *font=NULL;
	pstart();
	if ( mode==RES_LOAD )
	{
		font=TTF_OpenFont(path,(int)data);
		if ( font==NULL )
		{
			SET_ERROR(E_SDL,vstr(gettext("Failed to open a font '%s`: %s"),path,TTF_GetError()));
			return NULL;
		}
	}	else
	{
		TTF_CloseFont(data);
	}
	pstop();
	return font;
}

void *gfx_snd_control( const char *path, rescontrol_t mode, void *data )
{
	pstart();
	pstop();
	return NULL;
}

ecode_t gfx_draw( const SDL_Surface *src, SDL_Surface *dst, gfx_imgmode_t mode, \
		int x, int y, int _x, int _y, int _w, int _h )
{
	SDL_Rect rs, rd;
	pstart();
	
	rs.x=_x;
	rs.y=_y;
	rs.w=_w;
	rs.h=_h;
	if ( rs.w==0 ) rs.w=src->w;
	if ( rs.h==0 ) rs.h=src->h;
	
	if ( mode==GFX_IMG_PIECE )
	{
		rd.x=x;
		rd.y=y;
	}	else
	{
		rd.x=x+rs.x;
		rd.y=y+rs.y;
	}
	
	if ( SDL_BlitSurface((SDL_Surface*)src,&rs,dst,&rd)!=0 )
	{
		SET_ERROR(E_SDL,vstr(gettext("Failed to draw an image: %s"),SDL_GetError()));
		return E_SDL;
	}
	
	gfx_updated=1;
	
	pstop();
	return E_NONE;
}

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
