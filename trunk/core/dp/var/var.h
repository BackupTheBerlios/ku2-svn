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

typedef
struct STRUCT_VAR
{
	char *name;
	uint size;
}	var_t;

kucode_t var_init( ku_flag32_t flags );
void var_done( void );

kucode_t var_define( const char *name );
kucode_t var_define_val( const char *name, uint size, const void *data );
kucode_t var_redefine( const char *name, uint size, const void *data );
kucode_t var_undef( const char *name );

int var_defined( const char *name );

kucode_t var_get( const char *name, void *data );
void *var_addr( const char *name );

//! Assign flags and a control function for a resource type.
/*!
	Assigns flags and a control function for a resource type.
	\param type Resource type to deal with.
	\param flags Resource type flags.
	\param control Control function.
	\retval KE_NONE No error.
	\retval KE_MEMORY Memory allocation error.
	\retval KE_* abtree_ins() errors.
*/


#ifdef __cplusplus
}
#endif
#endif
