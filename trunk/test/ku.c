/***************************************************************************
 *            ku.c
 *
 *  Sat Sep  2 20:40:48 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "ku2/ku2.h"
#include "ku2/debug.h"
#include "ku2/ecode.h"
#include "ku2/gettext.h"
#include "ku2/memory.h"

#include "log/log.h"
#include "dialogue/dialogue.h"
#include "other/other.h"

#include "mm/mm.h"
#include "mm/gfx/gfx.h"
#include "mm/gui/gui.h"
#include "mm/widgets/frame.h"
#include "mm/widgets/gfxbut.h"

static kucode_t oper_gui( void )
{
	__label__ lerror;
	enum
	{
		RES_IMAGE
	};
	enum
	{
		INIT_SDL,
		INIT_RES
	}	init;
	pstart();
	
	//	SDL инициализация
	if ( SDL_Init(SDL_INIT_VIDEO) == -1 )
	{
		plog(gettext("Failed to initialize a SDL: %s\n"), SDL_GetError());
		return KE_EXTERNAL;
	}
	init = INIT_SDL;
	
	//	создание видео режима
	if ( gfx_create_window(640, 480, 32, 0, GFX_SOFTWARE, \
		"Kane Utilities 2 GUI testing", "KU2 GUI") != KE_NONE )
		goto lerror;
	
	//	загрузка ресурсов
	if ( res_init() != KE_NONE )
	{
		plog(gettext("Failed to initialize a resource manager: %d\n"), kucode);
		goto lerror;
	}
	init = INIT_RES;
	
	if ( res_assign(RES_IMAGE, 0, gfx_img_control) != KE_NONE )
	{
		plog(gettext("Failed to assign a control function to a " \
			"resource 'image`: %d\n"), kucode);
		goto lerror;
	}
	
	//	завершение
	res_halt();
	SDL_Quit();
	
	pstop();
	return KE_NONE;
	
	lerror:
	switch ( init )
	{
		case INIT_RES:
		{
			res_halt();
		}
		case INIT_SDL:
		{
			SDL_Quit();
		}
	}
	return kucode;
}

int main( void )
{
	char *oper;
	int finished = 0;
	pstart();
	
	dlgue_stream(stdin, stdout);
	
	if ( openlog("ku2.log") != KE_NONE )
	{
		dlgue_claim(gettext("Cannot open a log file: %s\n"), strerror(errno));
		return EXIT_FAILURE;
	}
	
	dlgue_claim(gettext("Welcome to Kane Utilities 2 Test programme!\n" \
		"\tCore version: %s\n" \
		"\tMulti-media version: %s\n"), STR(KU2_VERSION), STR(KU2MM_VERSION));
	plog(gettext("Kane Utilities 2, core %s, multi-media %s\n"), \
		STR(KU2_VERSION), STR(KU2MM_VERSION));
	
	/*
		Главный цикл
	*/
	while ( !finished )
	{
		dlgue_claim(gettext("Valid operations:\n\tgui - test GUI\n"));
		if ( dlgue_ask(gettext("Operation"), &oper, DLGUE_STRING) != KE_NONE )
		{
			dlgue_claim(gettext("Failed due to error %d.\n"), kucode);
			plog(gettext("Dialogue ask has failed: %d\n"), kucode);
			continue;
		}
		plog(gettext("Operation '%s` is chosen.\n"), oper);
		
		if ( !strcmp(oper, "quit") )
		{
			finished = 1;
		}	else
		if ( !strcmp(oper, "gui") )
		{
			if ( oper_gui() != KE_NONE )
				dlgue_claim(gettext("GUI testing has failed: %d\n"), kucode); else
				dlgue_claim(gettext("GUI testing has succeeded!\n"));
		}	else
		{
			dlgue_claim(gettext("Invalid operation.\n"));
		}
	}
	
	dlogmemstat();
	closelog();
	pstop();
	return EXIT_SUCCESS;
}
