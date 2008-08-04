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
	\date Wed Feb 07 10:33 2007
	\version 1.2.0
*/

#ifndef KU__TYPES_H__
#define KU__TYPES_H__
#ifdef __cplusplus
extern "C" {
#endif

//! 2 power 16
#define BIN16 (uint16_t)(-1)

//! 2 power 16 minus 1
#define BIN16m1 (BIN16-1)

//! 2 power 32
#define BIN32 (uint32_t)(-1)

//! 2 power 32 minus 1
#define BIN32m1 (BIN32-1)

#if !defined(NO_STDINT_H)
#include <stdint.h>

#elif !defined(NO_INTTYPES_H)
#include <inttypes.h>

#elif defined(WIN32) && defined(VCPP)
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
#else	// WIN32 && VCPP
	#error No integer types was found.
#endif	// WIN32 && VCPP

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
	\note \ref kucode should not be changed.
*/
typedef
int (*ku_comp_f)( const void *a, const void *b );

//! Interval function.
/*!
	This function type is used to calulate the intervals between data and left
	limit, and data and right limit.
	\param left Left limit or \e NULL for possible minimum.
	\param data Data or \e NULL to search for minimum or maximum.
	\param right Right limit or \e NULL for possible maximum.
	\param [out] interval Two integer`s array: \n
	\b interval[0] between data and left limit. \n
	\b interval[1] between data and right limit.
	\return If \b data is not \e NULL then \e NULL is returned. If both
		\b data is \e NULL then  the second minimum is returned.
	\note For instance for set of integers [1..20]: \n
	- For [3, 6, 12] function should set intervals to [2, 5]
		and return \e NULL. \n
	- For [NULL, 6, 7] function should set intervals to [5, 0]
		and return \e NULL. \n
	- For [NULL, NULL, 7] function should return pointer to 2.
	\note \ref kucode should not be changed.
*/
typedef
void *(*ku_interval_f)( const void *left, const void *data, \
					   const void *right, uint *interval );

//! Action function.
/*!
	This function type is used to perform some action on \e data.
	\param data Data pointer.
	\return Depends on the context.
	\note \ref kucode should not be changed.
*/
typedef
int (*ku_act_f)( void *data );

#ifdef __cplusplus
}
#endif
#endif
