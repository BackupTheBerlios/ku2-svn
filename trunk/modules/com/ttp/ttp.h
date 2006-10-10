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
	1.	��������� ��������� �� ���������
	2.	msg - ���������
	3.	���������� ���������� ����������,
		0, ���� ������,
		p_arg - ������ ��������� ����������
		p_argc - �� ����������
	4.	---
*/
int parse( char *msg );

#ifdef __cplusplus
}
#endif
#endif
