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

#if MM_BACKEND == SDL_OGL
	#if !defined(NO_NVIDIA_GL_H)
	#include <nvidia/GL/gl.h>
	#else
	#include <GL/gl.h>
	#endif
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
#include "gfx.h"

extern void glWindowPos2i( int x, int y );

kucode_t gfx_img_draw_adv( const gfx_image_t *img, gfx_imgmode_t mode,
						   int x, int y, int _x, int _y, int _w, int _h )
{
	SDL_Rect rs, rd;
	pstart();
	
	ku_avoid(img == NULL);
	
	rs.x = _x;
	rs.y = _y;
	rs.w = _w;
	rs.h = _h;
	if ( rs.w == 0 ) rs.w = img->w;
	if ( rs.h == 0 ) rs.h = img->h;
	
	if ( mode == GFX_IMG_PIECE )
	{
		rd.x = x;
		rd.y = y;
	}	else
	{
		rd.x = x+rs.x;
		rd.y = y+rs.y;
	}
	
	if ( SDL_BlitSurface((SDL_Surface*)img, &rs, gfx_screen, &rd) != 0 )
	{
		plogfn_i("IMAGE", gettext("Failed to draw an image: %s\n"), SDL_GetError());
		KU_ERRQ(KE_EXTERNAL);
	}
	
	gfx_needs_update = 1;
	preturn KE_NONE;
}

kucode_t gfx_img_draw( const gfx_image_t *img, int x, int y )
{
	pstart();
	_
	preturn gfx_img_draw_adv(img, GFX_IMG_PIECE, x, y, 0, 0, 0, 0);
	
	//glWindowPos2i(x, y);
	//glDrawPixels(src->w, src->h, src->format, GL_UNSIGNED_BYTE, src->pixels);
	
	//preturn KE_NONE;
}

void gfx_img_free( gfx_image_t *img )
{
	SDL_FreeSurface((SDL_Surface*)img);
}

gfx_image_t *gfx_img_fromSDL( SDL_Surface *src )
{
	gfx_image_t *img;
	uint oldy, newy;
	pstart();
	
	preturn (gfx_image_t*)src;
	
	SDL_LockSurface(src);
	img = dmalloc(sizeof(gfx_image_t)+ \
		src->pitch*src->h);
	if ( img == NULL )
	{
		plogfn_i("IMAGE", gettext("Out of memory.\n"));
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
	SDL_FreeSurface(src);
	
	preturn img;
}

static void *gfx_img_control( const char *path, rescontrol_t action, void *data )
{
	SDL_Surface *sdlimg;
	gfx_image_t *img;
	pstartp("path = %s, action = %s, data = %p",
			path,
			(action == RES_LOAD) ? "load": "unload",
			data);
	
	if ( action == RES_LOAD )
	{
		//	загрузка изображения
		/*sdl*/img = IMG_Load(path);
		if ( /*sdl*/img == NULL )
		{
			plogfn(gettext("Failed to load an image '%s`: %s\n"), path, IMG_GetError());
			preturn NULL;
		}
		
		//	создание gfx_image_t
		/*img = gfx_img_fromSDL(sdlimg);
		if ( img == NULL )
		{
			SDL_FreeSurface(sdlimg);
			preturn NULL;
		}*/
	}	else
	{
		gfx_img_free(img);
		img = NULL;
	}
	
	preturnp("img = %p", img) img;
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
