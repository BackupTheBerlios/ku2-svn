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

	//! DESCRIPTION.
typedef
struct STRUCT_VAR
{
	char *data;
	uint size;
}	var_t;

//! DESCRIPTION.
/*!
	DESCRIPTION.
	\param NAME DESC.
	\return DESC.
	\retval VAL DESC.
	\note NOTE.
	\sa SEE_ASLO.
*/
kucode_t var_init( ku_flag32_t flags );

//! DESCRIPTION.
/*!
	DESCRIPTION.
	\param NAME DESC.
	\return DESC.
	\retval VAL DESC.
	\note NOTE.
	\sa SEE_ASLO.
*/
void var_done( void );

//! DESCRIPTION.
/*!
	DESCRIPTION.
	\param NAME DESC.
	\return DESC.
	\retval VAL DESC.
	\note NOTE.
	\sa SEE_ASLO.
*/
kucode_t var_define( const char *name );

//! DESCRIPTION.
/*!
	DESCRIPTION.
	\param NAME DESC.
	\return DESC.
	\retval VAL DESC.
	\note NOTE.
	\sa SEE_ASLO.
*/
kucode_t var_define_val( const char *name, uint size, const void *data );

//! DESCRIPTION.
/*!
	DESCRIPTION.
	\param NAME DESC.
	\return DESC.
	\retval VAL DESC.
	\note NOTE.
	\sa SEE_ASLO.
*/
kucode_t var_redefine( const char *name, uint size, const void *data );

//! DESCRIPTION.
/*!
	DESCRIPTION.
	\param NAME DESC.
	\return DESC.
	\retval VAL DESC.
	\note NOTE.
	\sa SEE_ASLO.
*/
kucode_t var_undef( const char *name );

//! DESCRIPTION.
/*!
	DESCRIPTION.
	\param NAME DESC.
	\return DESC.
	\retval VAL DESC.
	\note NOTE.
	\sa SEE_ASLO.
*/
int var_defined( const char *name );

//! DESCRIPTION.
/*!
	DESCRIPTION.
	\param NAME DESC.
	\return DESC.
	\retval VAL DESC.
	\note NOTE.
	\sa SEE_ASLO.
*/
kucode_t var_get( const char *name, void *data );

//! DESCRIPTION.
/*!
	DESCRIPTION.
	\param NAME DESC.
	\return DESC.
	\retval VAL DESC.
	\note NOTE.
	\sa SEE_ASLO.
*/
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
