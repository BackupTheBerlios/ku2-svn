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

//! Valid error codes.
/*!	
	A list of the valid error codes for the Kane Utilities.
*/
typedef
enum
{
	KE_NONE,		//!< No error.
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

//! Contains code of the last error.
/*!
	This variable contains the code of the last error
	after the call of a function unless functions is said not
	to set this variable.
*/
extern kucode_t kucode;

//! Set the error code and return it from the function.
#define KU_ERRQ( __ecode ) \
{ \
	kucode = __ecode; \
	return __ecode; \
}

#ifdef __cplusplus
}
#endif
#endif
