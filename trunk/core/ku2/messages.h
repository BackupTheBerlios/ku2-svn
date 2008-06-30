/*
		messages.h
		Mon Apr 21 18:16:59 2008

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2008
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

/*!
	\file
	\brief BRIEF.
	
	DESC.
	\author J. Anton
*/

#ifndef KU__MESSAGES_H__
#define KU__MESSAGES_H__
#ifdef __cplusplus
extern "C" {
#endif

#define KU_MESSAGE __ku_message

#define KU_MESSAGE_TYPE( __type ) \
__type KU_MESSAGE

#define KU_WATCH \
{ \
	__label__ __ku_parse_end; \
	{ \
		__label__ __ku_parse_begin;

#define KU_WATCH_CMSG_DEFS \
{ \
	__label__ __ku_parse_end; \

#define KU_DEFINE_CMSG( __message ) \
	__label__ __ku_parse_w##__message

#define KU_WATCH_CMSG_BEGIN \
	{ \
		__label__ __ku_parse_begin;

#define KU_PARSE \
		goto __ku_parse_end; \
		__ku_parse_begin:; \
	}

#define KU_PARSE_CMSG( __message ) \
__ku_parse_w##__message

#define KU_BREAK_PARSE \
goto __ku_parse_end

#define KU_ENDWATCH \
	__ku_parse_end:; \
} asm("")

#define KU_MAKEMSG( __message ) \
{ \
	KU_MESSAGE = __message; \
	goto __ku_parse_begin; \
}

#define KU_MAKEMSG_() \
goto __ku_parse_begin

#define KU_MKCMSG( __message ) \
goto __ku_parse_w##__message

#define KU_OPEN_BLOCK \
{ \
	__label__ __ku_block_end;

#define KU_CLOSE_BLOCK \
	__ku_block_end:; \
}

#define KU_LEAVE_BLOCK \
goto __ku_block_end

	
	
	
	
#define KU_DEFINE_TAIL( __id ) \
__label__ __ku_tail_W##__id

#define KU_BEGIN_TAIL \
{ \
	__label__ __ku_tail_end; \
	goto __ku_tail_end \

#define KU_END_TAIL \
	__ku_tail_end:; \
}

#define KU_TAIL( __id ) \
__ku_tail_W##id
	
	
	
	
	
#ifdef __cplusplus
}
#endif
#endif
