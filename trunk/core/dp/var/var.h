/*
		var.h
		Sun Apr 15 15:33:50 2007

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2007
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

/*!
	\file
	\brief Variable manager.
	
	Variable handling routines.
	\author J. Anton
	\date Sun Apr 15 15:34:46 2007
	\version 1.0.0
*/

#ifndef KU__VAR_H__
#define KU__VAR_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/ecode.h"
#include "ku2/types.h"

#define VAL_INTEGER		'i'
#define VAL_LONGINT		'I'
#define VAL_DOUBLE		'f'
#define VAL_BOOLEAN		'b'
#define VAL_STRING		's'

typedef
struct STRUCT_VAR
{
	char *name;
	char *val_types;
	void **values;
	uint vals, size;
}	var_t;

// Define and allocate the memory for new variable
var_t *var_define( const char *name, const char *val_types, ... );

// Undefine the variable
kucode_t var_undef( var_t *var );

#ifdef __cplusplus
}
#endif
#endif
