/***************************************************************************
 *            font.c
 *
 *  Sun Sep 10 18:45:37 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <string.h>

#include "font.h"
#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/gettext.h"
#include "ku2/memory.h"

#include "dp/resmanager/res.h"
#include "ds/abtree/abtree.h"
#include "io/log/log.h"

tree_t *fonts;

static void *font_control( const char *path, rescontrol_t action, void *data )
{
	TTF_Font *font;
	
	if ( action == RES_LOAD )
	{
		font = TTF_OpenFont(path, *((int*)data));
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
	
	return font;
}

static int font_cmpf( void *font1, void *font2 )
{
	gfx_font_t *const f1 = font1,
		*const f2 = font2;
	int i;
	
	i = strcmp(f1->name, f2->name);
	if ( i != 0 )
		return i;
	
	if ( f1->style != f2->style )
		return f1->style-f2->style;
	
	return f1->size-f2->size;
}

static int font_freef( void *font )
{
	TTF_CloseFont(((gfx_font_t*)font)->font);
	dfree(font);
	return 1;
}

kucode_t font_init( int typeID )
{
	pstart();
	
	if ( TTF_Init() != 0 )
	{
		plog(gettext("Failed to initialize a SDL_ttf: %s\n"), TTF_GetError());
		KU_ERRQ(KE_LIBRARY);
	}
	
	if ( res_assign(typeID, RESTYPE_UNIQ, font_control) != KE_NONE )
	{
		plog(gettext("Failed to assign a font resource control function.\n"));
		TTF_Quit();
		return kucode;
	}
	
	fonts = abtree_create(font_cmpf, 0);
	if ( fonts == NULL )
	{
		plog(gettext("Failed to create a tree.\n"));
		TTF_Quit();
		return kucode;
	}
	
	pstop();
	return KE_NONE;
}

kucode_t font_halt( void )
{
	pstart();
	
	abtree_free(fonts, font_freef);
	TTF_Quit();
	
	pstop();
	return KE_NONE;
}

gfx_font_t *font_open( const char *name, gfx_font_style_t style, int size )
{
	gfx_font_t font_search;
	gfx_font_t *font;
	pstart();
	
	// поиск шрифта
	font_search.name = (char*)name;
	font_search.style = style;
	font_search.size = size;
	font = abtree_search(fonts, &font_search);
	
	if ( font )
	{
		font->instances++;
	}	else
	{
		// загрузка шрифта
		font = dmalloc(sizeof(gfx_font_t)+sizeof(char)*(strlen(name)+1));
		if ( font == NULL )
		{
			kucode = KE_MEMORY;
			return NULL;
		}
		
		font->font = res_access_adv(name, &size);
		if ( font->font == NULL )
		{
			dfree(font);
			plog(gettext("Failed to access a font resource.\n"));
			return NULL;
		}
		
		// добавление в дерево
		if ( abtree_ins(fonts, font) != KE_NONE )
		{
			TTF_CloseFont(font->font);
			dfree(font);
			return NULL;
		}
		
		// установка параметров
		TTF_SetFontStyle(font->font, style);
		font->name = (char*)((int8_t*)font+sizeof(gfx_font_t));
		strcpy(font->name, name);
		font->style = style;
		font->size = size;
		font->instances = 1;
	}
	
	pstop();
	return font;
}

kucode_t font_close( gfx_font_t *font )
{
	pstart();
	
	if ( font->instances == 1 )
	{
		TTF_CloseFont(font->font);
		if ( abtree_rem(fonts, font, NULL) != KE_NONE )
			plog(gettext("Note: failed to remove a font '%s` from the tree: %d.\n"),
				font->name, kucode);
		dfree(font);
	}	else
	{
		font->instances--;
	}
	
	pstop();
	return KE_NONE;
}

gfx_font_t *font_change( gfx_font_t *font, gfx_font_style_t style, int size )
{
	pstart();
	
	pstop();
	return NULL;
}
