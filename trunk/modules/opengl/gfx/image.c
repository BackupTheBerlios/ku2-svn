/*
		image.c
		Sun Nov 25 16:56:23 2007

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2007
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

#include <string.h>
#include <errno.h>

#if !defined(NO_NVIDIA_GL_H)
#include <nvidia/GL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "SDL.h"
#include "SDL_image.h"

#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/memory.h"
#include "ku2/gettext.h"
#include "io/log/log.h"
#include "dp/resmanager/res.h"
#include "io/cfgreader/cfg.h"

#include "image.h"

extern void glWindowPos2i( int x, int y );

kucode_t gfx_image_draw( gfx_image_t *src, int x, int y )
{
	pstart();
	
	glWindowPos2i(x, y);
	glDrawPixels(src->w, src->h, src->format, GL_UNSIGNED_BYTE, src->pixels);
	
	preturn KE_NONE;
}

gfx_image_t *gfx_img_fromSDL( SDL_Surface *src )
{
	gfx_image_t *img;
	uint oldy, newy;
	pstart();
	
	SDL_LockSurface(src);
	img = dmalloc(sizeof(gfx_image_t)+ \
		src->pitch*src->h);
	if ( img == NULL )
	{
		plogfn(gettext("Out of memory.\n"));
		SDL_FreeSurface(src);
		preturn NULL;
	}
	
	img->w = src->w;
	img->h = src->h;
	switch ( src->format->BitsPerPixel )
	{
		case 24:
		{
			img->format = GL_RGB;
			break;
		}
		case 32:
		{
			img->format = GL_RGBA;
			break;
		}
		default:
		{
			plogfn(gettext("Not supported image pixel format: %d bits.\n"), \
				src->format->BitsPerPixel);
			dfree(img);
			return NULL;
		}
	}
	img->pixels = (int8_t*)img+sizeof(gfx_image_t);
	
	for ( oldy = img->h-1, newy = 0; newy < img->h; oldy--, newy++ )
		memmove((int8_t*)(img->pixels)+newy*src->pitch, \
			(int8_t*)(src->pixels)+oldy*src->pitch, \
			src->pitch);
	
	SDL_UnlockSurface(src);
	
	preturn img;
}

static void *gfx_img_control( const char *path, rescontrol_t action, void *data )
{
	SDL_Surface *sdlimg;
	gfx_image_t *img;
	pstart();
	
	if ( action == RES_LOAD )
	{
		//	загрузка изображения
		sdlimg = IMG_Load(path);
		if ( sdlimg == NULL )
		{
			plogfn(gettext("Failed to load an image '%s`: %s\n"), path, IMG_GetError());
			preturn NULL;
		}
		
		//	создание gfx_image_t
		img = gfx_img_fromSDL(sdlimg);
		if ( img == NULL )
		{
			SDL_FreeSurface(sdlimg);
			preturn NULL;
		}
		
		//	удаление SDL_Surface
		SDL_FreeSurface(sdlimg);
	}	else
	{
		dfree(data);
		img = NULL;
	}
	
	preturn img;
}

kucode_t gfx_add_image_resources( int resource_type, const char *resfile )
{
	uint i = 0;
	char img_id[CFG_BUFFER],
		img_file[CFG_BUFFER];
	kucode_t code;
	cfg_session_t *cfg;
	pstart();
	
	if ( res_assign(resource_type, RESTYPE_ZFL, gfx_img_control) != KE_NONE )
	{
		plogfn(gettext("Failed to assign an image resource type: %d.\n"), KU_GET_ERROR());
		preturn KU_GET_ERROR();
	}
	
	if ( (cfg = cfg_open(resfile, CFG_STRICT|CFG_STEP)) == NULL )
	{
		if ( KU_GET_ERROR() == KE_IO )
			plogfn(gettext("Failed to open a file: %s.\n"), strerror(errno)); else
			plogfn(gettext("Failed to open a file: %d.\n"), KU_GET_ERROR());
		preturn KU_GET_ERROR();
	}
	
	if ( cfg_query(cfg, "image=ss/r", img_id, img_file) != KE_NONE )
	{
		plogfn(gettext("Failed to add a query for the resource file: %d.\n"), KU_GET_ERROR());
		cfg_close(cfg);
		preturn KU_GET_ERROR();
	}
	
	while ( (code = cfg_process(cfg)) == KE_SIGNAL )
	{
		if ( res_add(img_file, img_id, resource_type, NULL, 0) != KE_NONE )
		{
			plogfn(gettext("Failed to add an image resource (line %d): %d.\n"), \
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
	
	plogfn(gettext("%d images added.\n"), i);
	
	preturn KE_NONE;
}
