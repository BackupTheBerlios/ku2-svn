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

#include <sys/types.h>

//! 32 bit flags.
typedef
u_int32_t ku_flag32_t;

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
