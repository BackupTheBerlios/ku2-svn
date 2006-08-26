/***************************************************************************
 *            gfx.h
 *
 *  Thu Jun  1 12:18:51 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#ifndef __GFX_H__
#define __GFX_H__

#include "SDL.h"
#include "SDL_ttf.h"
#include "errors.h"
#include "ku2/resmanager/res.h"

/*
	Режим экрана
*/
typedef
enum
{
	GFX_SOFTWARE,
	GFX_HARDWARE
}	gfx_mode_t;

/*
	Выравнивание текста
*/
typedef
enum
{
	GFX_ALLIG_LEFT,
	GFX_ALLIG_MIDDLE,
	GFX_ALLIG_RIGHT
}	gfx_txtallig_t;

/*typedef
enum
{
	GFX_TEXT_ONELINE,
	GFX_TEXT_MULTILINE
}	gfx_txtmode_t;*/

/*
	Стиль текста, B.U.I.
*/
typedef
int gfx_txtstyle_t;

/*
	Режим рисования
*/
typedef
enum
{
	/*
		указанный кусок будет выводиться в
		указанных координатах (для спрайтов)
	*/
	GFX_IMG_PIECE,
	
	/*
		будет выводиться часть изображения в том
		месте, где бы оно было при выводе всего
		изображения
	*/
	GFX_IMG_REAL
}	gfx_imgmode_t;

/*
	Наиболее близкий стандартный режим
*/
typedef
enum
{
	GFX_RES_640,
	GFX_RES_800
}	gfx_resolution_t;

//	Экран
extern SDL_Surface *screen;

//	Нужно ли обновлять экран
extern int gfx_updated;

//	Наиболее близкий стандартный режим
extern gfx_resolution_t gfx_resolution;

/*
	1.	Создание видео режима
	2.	width - ширина экрана
		height - высота экрана
		bpp - режим цвета
		fullscreen - устонавливать ли полноэкранный режим
		mode - режим видео устройства
	3.	---
	4.	E_INVALID - невозможно подобрать стандартный режим
		E_SDL - ошибка функций SDL
*/
ecode_t gfx_create_window( int width, int height, int bpp, int fullscreen, gfx_mode_t mode );

/*
	1.	Функции контроля для менеджера ресурсов
	2.	path - путь к файлу
		mode - тип действия (загрузить/выгрузить)
		data - выгружаемый объект или параметр загрузки объекта
	3.	a) загруженый объект или NULL в случае ошибки
		b) NULL, если объект удалось выгрузить
	4.	E_SDL - ошибка функций SDL
*/
void *gfx_img_control( const char *path, rescontrol_t mode, void *data );
void *gfx_fnt_control( const char *path, rescontrol_t mode, void *data );
void *gfx_snd_control( const char *path, rescontrol_t mode, void *data );

/*
	1.	Нарисовать одно изображение на другом
	2.	src - исходное изображение
		dst - назначение
		mode - режим рисования
		x, y - координаты назначения
		_x, _y, _w, _h - участок исходного объекта
	3.	---
	4.	E_SDL - ошибка функций SDL
*/
ecode_t gfx_draw( const SDL_Surface *src, SDL_Surface *dst, gfx_imgmode_t mode, \
		int x, int y, int _x, int _y, int _w, int _h );

/*
	1.	Создать изображение с надписью
	2.	text - текст
		font - шрифт
		style - стиль шрифта
		colour - цвет шрифта
	3.	Холст с изображением или NULL в случае ошибки
	4.	E_SDL - ошибка функций SDL
*/
SDL_Surface *gfx_txtrender ( const char *text, const TTF_Font *font, gfx_txtstyle_t style, SDL_Color colour );

/*
	1.	Написать текст на изображении
	2.	text - текст
		font - шрифт
		dst - изображение назначения
		style - стиль шрифта
		colour - цвет шрифта
		x, y - координаты назначения
		_x, _y, _w, _h - участок исходного текста
	3.	---
	4.	ошибки gfx_txtrender и gfx_draw
*/
ecode_t gfx_print( const char *text, const TTF_Font *font, SDL_Surface *dst, gfx_txtstyle_t style, \
		SDL_Color colour, int x, int y, int _x, int _y, int _w, int _h );

/*
	NOT IMPLEMENTED
*/
ecode_t gfx_printf( const char *text, const TTF_Font *font, SDL_Surface *dst, gfx_txtstyle_t style, \
		SDL_Color colour, int x, int y, int w, int h, gfx_txtallig_t allig );

#endif
