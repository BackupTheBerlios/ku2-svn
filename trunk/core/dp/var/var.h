/*
		var.h
		Sat Jul 14 21:21:59 2007

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2007
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

/*!
	\file
	\brief Variable handling routines.
	
	Variable handling routines.
	\author J. Anton
	\version 1.6.4
*/

#ifndef KU__VAR_H__
#define KU__VAR_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/ecode.h"

//! Value is 'int`.
#define VAL_INTEGER		'i'
//! Value is 'long int`.
#define VAL_LONGINT		'I'
//! Value is 'double`.
#define VAL_DOUBLE		'f'
//! Value is 'boolean (1/0 int)`.
#define VAL_BOOLEAN		'b'
//! Value is 'string (char*)`.
#define VAL_STRING		's'

//! Variable structure.
typedef
struct STRUCT_VAR
{
	char *name;		//!< Variable name.
	char *val_types;//!< List of variable value types.
	void **values;	//!< List of pointers to variable values.
}	var_t;

//! Define and allocate the memory for new variable.
/*!
	Defines and allocates the memory for new variable.
	\param name Variable name.
	\param val_types List of types of variable parameters (see VAL_* defines).
	\param ... List of parameters.
	\return Pointer to the new allocated variable, else \e NULL and \ref kucode
	is set to the valid value.
	\note Possible \ref kucode values are the same as in \ret var_define_v().
	\sa var_define_v() and var_undef().
*/
var_t *var_define( const char *name, const char *val_types, ... );

#ifdef _STDARG_H
//! Define and allocate the memory for new variable (va_list version).
/*!
	Defines and allocates the memory for new variable.
	\param name Variable name.
	\param val_types List of types of variable parameters (see VAL_* defines).
	\param var_ap Varying parameter list identifier.
	\return Pointer to the new allocated variable, else \e NULL and \ref kucode
	is set to the valid value.
	\note Possible \ref kucode values: \n
	\ref KE_INVALID Invalid parameter type. \n
	\ref KE_MEMORY Memory allocation has failed.
	\note Header 'stdarg.h` should be included before the 'var.h` in
	order to use this function.
	\sa var_define() and var_undef().
*/
var_t *var_define_v( const char *name, const char *val_types, va_list var_ap );
#endif

//! Undefine the variable.
/*!
	Undefines and freed the allocated memory of the variable.
	\param var Variable to be undefined.
	\return Always \ref KE_NONE.
	\sa var_define().
*/
// Undefine the variable
kucode_t var_undef( var_t *var );

#ifdef __cplusplus
}
#endif
#endif
