/*
		directives.h
		Mon May 21 19:33:35 2007

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2007
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

/*!
	\file
	\brief BRIEF.
	
	DESC.
	\author J. Anton
	\date Mon May 21 19:33:46 2007
	\version 1.6.0
*/

#ifndef KU__DIRECTIVES_H__
#define KU__DIRECTIVES_H__
#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG

#include "ku2/debug.h"

#define KU_CALL_ONCE \
{ \
	static __once = 0; \
	if ( __once == 1 ) \
		pdebug("KU_CALL_ONCE directive is ignored!\n"); \
	__once = 1; \
}

#else

#define KU_CALL_ONCE

#endif

#ifdef __cplusplus
}
#endif
#endif
