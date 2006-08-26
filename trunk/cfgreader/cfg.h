/***************************************************************************
 *            cfg.h
 *
 *  Fri Aug 25 21:30:11 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#ifndef KU__CFG_H__
#define KU__CFG_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/ecode.h"

#define CFG_BUFFER 2048

/*
	Один запрос
*/
typedef
struct
{
	char *id;
	char *fmt;
	void **ptr;
}	cfg_query_t;

/*
	секущая строчка
*/
extern int cfg_line;

/*
	Открыть файл
*/
kucode_t cfg_open( char *file );

/*
	Закрыть файл
*/
kucode_t cfg_close( void );

/*
	Добавить запрос
	Формат:
		i: int
		f: double
		s: char* (string)
		b: int (boolean 1/0)
	... указываются указатели на переменные, этот
	модуль переменные не создаёт, а работает с существующими
*/
kucode_t cfg_query( char *id, char *fmt, ... );

/*
	Выполнить запросы
	ВНИМАНИЕ!!
		при чтении запроса, в котором есть строки,
		НЕ проверяется размер строки назначения, но известно,
		что эта строка не может быть больше CFG_BUFFER символов
*/
kucode_t cfg_process( void );

#include "errors/close_code.h"
#endif
