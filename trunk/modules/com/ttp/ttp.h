/*!
	\file
	\brief Text Transfer Protocol.
	
	Communication protocol for text data transfer.
	\author J. Anton
	\date
	\version 0.1.0
*/

#ifndef KU__TTP_H__
#define KU__TTP_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/ecode.h"
#include "ku2/types.h"

#define PARSER_ARG_CNT 16
extern char *p_arg[PARSER_ARG_CNT];
extern int p_argc;

/*
	1.	–азбиение сообщени€ на параметры
	2.	msg - сообщение
	3.	 оличество прочитаных параметров,
		0, если ошибка,
		p_arg - список полученых параметров
		p_argc - их количество
	4.	---
*/
int parse( char *msg );

#ifdef __cplusplus
}
#endif
#endif
