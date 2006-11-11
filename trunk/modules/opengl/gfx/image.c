/***************************************************************************
 *            image.c
 *
 *  Mon Oct 23 18:04:04 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <string.h>
#include <errno.h>

#include "SDL.h"
#include "SDL_image.h"

#include "image.h"
#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/gettext.h"
#include "io/log/log.h"
#include "dp/resmanager/res.h"
#include "io/cfgreader/cfg.h"

static void *gfx_img_control( const char *path, rescontrol_t action, void *data )
{
	SDL_Surface *img;
	pstart();

	if ( action == RES_LOAD )
	{
		img = IMG_Load(path);
		if ( img == NULL )
		{
			plogfn(gettext("Failed to load an image '%s`: %s\n"), path, IMG_GetError());
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
