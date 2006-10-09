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

#include "modules/com/channel/channel.h"

#if 0
#include "modules/mm/mm.h"
#include "modules/mm/gfx/gfx.h"
#include "modules/mm/gui/gui.h"
#include "modules/mm/widgets/frame.h"
#include "modules/mm/widgets/gfxbut.h"

static int running;
static gui_obj_t *window,
	*button1, *button2, *button3;

static kucode_t oper_gui_window( void )
{
	pstart();
	
	window = gui_obj_create(frame_init, sizeof(gui_frame_t), GUI_KEEPCHILDREN);
	if ( window == NULL )
	{
		plog(gettext("Failed to create window: %d\n"), kucode);
		return kucode;
	}
	if ( gui_set(window, FRAME_BACKGROUND, "menu back") != KE_NONE )
	{
		plog(gettext("Failed to set an attribute of the window: %d\n"), kucode);
		gui_obj_delete(window);
		return kucode;
	}
	if ( gui_move(window, NULL, 0, 0, 800, 600) != KE_NONE )
	{
		plog(gettext("Failed to place a window: %d\n"), kucode);
		gui_obj_delete(window);
		return kucode;
	}
	if ( gui_ch_status(window, GUI_ENABLED) != KE_NONE )
	{
		plog(gettext("Failed to load a window: %d\n"), kucode);
		gui_obj_delete(window);
		return kucode;
	}
	if ( gui_draw(window, 0, 0, 0, 0) != KE_NONE )
	{
		gui_obj_delete(window);
		return kucode;
	}
	
	pstop();
	return KE_NONE;
}

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
		INIT_RES,
		INIT_GUI
	}	init;
	SDL_Event event;
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
	if ( (res_add("test/data/images/background.png", \
		"menu back", RES_IMAGE, NULL, 0) != KE_NONE) || \
		(res_add("test/data/images/gfxbut_nor.png", "button", \
		RES_IMAGE, NULL, 0) != KE_NONE) || \
		(res_add("test/data/images/gfxbut_mon.png", "button on", \
		RES_IMAGE, NULL, 0) != KE_NONE) || \
		(res_add("test/data/images/gfxbut_md.png", "button down", \
		RES_IMAGE, NULL, 0) != KE_NONE) )
	{
		plog(gettext("Failed to add resources: %d\n"), kucode);
		goto lerror;
	}
	
	//	инициализация GUI
	if ( gui_init() != KE_NONE )
		goto lerror;
	init = INIT_GUI;
	
	//	загрузка окна
	if ( oper_gui_window() != KE_NONE )
		goto lerror;
	SDL_Flip(screen);
	
	//	цикл GUI
	running = 1;
	while ( running )
	{
		if ( SDL_WaitEvent(&event) )
		{
			switch ( gui_process(&event) )
			{
				case GUIE_LEAVE:
				{
					switch ( event.type )
					{
						case SDL_QUIT:
							running = 0;
					}
					break;
				}
			}
		}	else
		{
			plog(gettext("Failed for waiting for an event: %s\n"), SDL_GetError());
		}
	}
	
	//	завершение
	gui_halt();
	res_halt();
	SDL_Quit();
	
	pstop();
	return KE_NONE;
	
	lerror:
	switch ( init )
	{
		case INIT_GUI:
		{
			gui_halt();
		}
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
#endif

int main( int argc, char *argv[] )
{
	char *oper;
	int finished = 0;
	channel_t *chan;
	pstart();
	
	chan = channel_assign(0, 3, 0, 0);
	channel_write(chan, 0, "hello world!!", 14);
	channel_write(chan, 1, "qwe", 4);
	channel_write(chan, 0, "ttt", 4);

	#if 0
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
	#endif
	pstop();
	return EXIT_SUCCESS;
}
