/***************************************************************************
 *            types.h
 *
 *  Tue Aug 22 21:12:03 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Type definitions.
	\author J. Anton
	\date Tue Aug 22 21:17:10 2006
	\version 1.1.0
*/

#ifndef KU__TYPES_H__
#define KU__TYPES_H__
#ifdef __cplusplus
extern "C" {
#endif

#if !defined(NO_STDINT_H)
#include <stdint.h>

#elif !defined(NO_INTTYPES_H)
#include <inttypes.h>

#else	// No integer types
#ifdef WIN32
//! Integer type definitions for VC++.
/*!
	Integer types for VC++.
	\{
*/
typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
/*!
	\}
*/
#else	// WIN32
	#error No integer types was found.
#endif	// WIN32

#endif	// No integer types

//! Unsigned integer.
typedef
unsigned int uint;

//! 32 bit flags.
typedef
uint32_t ku_flag32_t;

//! Comparing function.
/*!
	This function type is used by data containers to compare two pieces of data.
	\param a First data.
	\param b Second data.
	\retval >0 First data are more than the second ones.
	\retval 0 Data are equal.
	\retval <0 First data are less than the second ones.
*/
typedef
int (*ku_comp_f)( void *a, void *b );

//! Action function.
/*!
	This function type is used to perform some action on \e data.
	\param data Data pointer.
	\return Depends on the context.
*/
typedef
int (*ku_act_f)( void *data );

#ifdef __cplusplus
}
#endif
#endif
