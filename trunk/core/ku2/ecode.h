/***************************************************************************
 *            ecode.h
 *
 *  Thu Aug 17 22:58:52 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Error code related definitions.
	
	This file contains the definitions of data related
	to the error management system of the Kane Utilities.
	\author J. Anton
	\date Thu Aug 17 23:03:09 2006
	\version 1.1.0
*/

#ifndef KU__ECODE_H__
#define KU__ECODE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/debug.h"

//! Valid error codes.
/*!	
	A list of the valid error codes for the Kane Utilities.
*/
typedef
enum
{
	KE_NONE,		//!< No error.
	KE_NOIMPLEM,	//!< Feature is not implemented yet.
	KE_SIGNAL,		//!< Signal message.
	KE_ASSERT,		//!< Assertion has failed.
	KE_MEMORY,		//!< Memory allocation error.
	KE_EMPTY,		//!< Data container is empty.
	KE_FULL,		//!< Data container is full.
	KE_NOTFOUND,	//!< Data was not found.
	KE_DOUBLE,		//!< Data already exists.
	KE_IO,			//!< Input/Output error.
	KE_SYNTAX,		//!< Syntax error.
	KE_EXTERNAL,	//!< External error.
	KE_INVALID,		//!< Invalid data.
	KE_LIBRARY		//!< External library error.
}	kucode_t;

#ifdef ALLOW_KUCODE_ACCESS
//! Contains code of the last error.
/*!
	This variable contains the code of the last error
	after the call of a function unless functions is said not
	to set this variable.
*/
extern kucode_t kucode;

//! Set the error code.
#define KU_SET_ERROR( __ecode ) \
kucode = __ecode

//! Get the error code.
#define KU_GET_ERROR() \
kucode

#else	// ALLOW_KUCODE_ACCESS
kucode_t ku_set_error( kucode_t __ecode );
kucode_t ku_get_error( void );

#define KU_SET_ERROR( __ecode ) \
ku_set_error(__ecode)

#define KU_GET_ERROR() \
ku_get_error()

#endif	// ALLOW_KUCODE_ACCESS

//! Return the last error code from the function.
#define KU_ERRQ_PASS() \
preturn KU_GET_ERROR()

//! Set the error code and return it from the function.
#define KU_ERRQ( __ecode ) \
preturn (KU_SET_ERROR(__ecode));

//! Set the error code and return the value from the function.
#define KU_ERRQ_VALUE(__ecode, __value) \
{ \
	KU_SET_ERROR(__ecode); \
	preturn __value; \
}

#define KU_ERRQ_BLOCKED() \
KU_ERRQ_PASS()

#define KU_WITHOUT_ERROR(__call) __call
#define KU_WITHOUT_ERROR_START()
#define KU_WITHOUT_ERROR_STOP()

#ifdef __cplusplus
}
#endif
#endif
