/*
		gfx.c
		Tue Dec 25 17:13:53 2007

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2007
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

#include "SDL.h"

#include "io/log/log.h"
#include "ku2/ecode.h"
#include "ku2/gettext.h"

#include "gfx.h"

SDL_Surface *gfx_screen;

int gfx_updated = 0;
gfx_resolution_t gfx_resolution;

int gfx_needs_update;

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
		plogfn_i("GFX", gettext("Invalid resolution: %dx%dx%d.\n"), width, height, bpp);
		KU_ERRQ(KE_INVALID);
	}
	
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
		case GFX_OPENGL:
		{
			KU_ERRQ(KE_NOIMPLEM);
			break;
		}
	}
	
	if ( fullscreen )
		flags |= SDL_FULLSCREEN;
	
	gfx_screen = SDL_SetVideoMode(width, height, bpp, flags);
	if ( gfx_screen == NULL )
	{
		plogfn_i("GFX", gettext("Failed to set the video mode: %s\n"), SDL_GetError());
		KU_ERRQ(KE_EXTERNAL);
	}
	
	SDL_WM_SetCaption(caption, bar_caption);
	
	vi = SDL_GetVideoInfo();
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
	
	preturn KE_NONE;
}

void gfx_screen_update( void )
{
	if ( gfx_needs_update )
	{
		SDL_Flip(gfx_screen);
		gfx_needs_update = 0;
	}
}
