/***************************************************************************
 *            image.c
 *
 *  Mon Oct 23 18:04:04 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <string.h>
#include <errno.h>

#if !defined(NO_NVIDIA_GL_H)
#include <nvidia/GL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "SDL.h"
#include "SDL_image.h"

#include "image.h"
#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/memory.h"
#include "ku2/gettext.h"
#include "io/log/log.h"
#include "dp/resmanager/res.h"
#include "io/cfgreader/cfg.h"

gfx_image_t *gfx_img_fromSDL( SDL_Surface *src )
{
	gfx_image_t *img;
	int oldy, newy;
	pstart();

	SDL_LockSurface(src);
	img = dmalloc(sizeof(gfx_image_t)+ \
		src->pitch*src->h);
	if ( img == NULL )
	{
		plogfn(gettext("Out of memory.\n"));
		SDL_FreeSurface(src);
		return NULL;
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

	pstop();
	return img;
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
			return NULL;
		}

		//	создание gfx_image_t
		img = gfx_img_fromSDL(sdlimg);
		if ( img == NULL )
		{
			SDL_FreeSurface(sdlimg);
			return NULL;
		}

		//	удаление SDL_Surface
		SDL_FreeSurface(sdlimg);
	}	else
	{
		dfree(data);
		img = NULL;
	}

	pstop();
	return img;
}

kucode_t gfx_add_image_resources( int resource_type, const char *resfile )
{
	uint i = 0;
	char img_id[CFG_BUFFER],
		img_file[CFG_BUFFER];
	kucode_t code;
	pstart();

	if ( res_assign(resource_type, RESTYPE_ZFL, gfx_img_control) != KE_NONE )
	{
		plogfn(gettext("Failed to assign an image resource type: %d.\n"), kucode);
		return kucode;
	}

	if ( cfg_open(resfile) != KE_NONE )
	{
		if ( kucode == KE_IO )
			plogfn(gettext("Failed to open a file: %s.\n"), strerror(errno)); else
			plogfn(gettext("Failed to open a file: %d.\n"), kucode);
		return kucode;
	}

	if ( cfg_query("image", "ss", img_id, img_file) != KE_NONE )
	{
		plogfn(gettext("Failed to add a query for the resource file: %d.\n"), kucode);
		cfg_close();
		return kucode;
	}

	while ( (code = cfg_process(CFG_STRICT|CFG_STEP)) == KE_SIGNAL )
	{
		if ( res_add(img_file, img_id, resource_type, NULL, 0) != KE_NONE )
		{
			plogfn(gettext("Failed to add an image resource (line %d): %d.\n"), \
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

	plogfn(gettext("%d images added.\n"), i);

	pstop();
	return KE_NONE;
}
