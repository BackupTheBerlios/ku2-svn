/*
 *	ecode.h
 *
 * This file is the part of Kane Utilities 2.
 * See licensing agreement in a root directory for more details.
 * http://developer.berlios.de/projects/ku2/
 *
 * Copyright, 2006
 *	J. Anton (Jeļkins Antons) aka Kane
 *	kane@mail.berlios.de
 */

/*!
 * \file
 * \author J. Anton
 * \date Thu Aug 17 23:03:09 2006
 * \brief Error code related definitions.
 *
 * This file contains the definitions of data related
 * to the error management system of the Kane Utilities.
 */

#ifndef KU__ECODE_H__
#define KU__ECODE_H__
#include "host.h"
KU_BEGIN_DECLS

#include "ku2/debug.h"

//! Valid error codes.
/*!
 * A list of the valid error codes for the Kane Utilities.
 */
typedef
enum
{
	KE_NONE,               //!< No error.
	KE_NOIMPLEM,           //!< Feature is not implemented yet.
	KE_SIGNAL,             //!< No error (alternative signal message).
	KE_ASSERT,             //!< Assertion has failed.
	KE_MEMORY,             //!< Memory allocation error.
	KE_EMPTY,              //!< Data container is empty.
	KE_FULL,               //!< Data container is full.
	KE_NOTFOUND,           //!< Data was not found.
	KE_DOUBLE,             //!< Data already exists.
	KE_IO,                 //!< Input/Output error.
	KE_SYNTAX,             //!< Syntax error.
	KE_EXTERNAL,           //!< External error.
	KE_INVALID,            //!< Invalid data.
	KE_LIBRARY,            //!< External library error.
	KE_UNDERFLOW,          //!< Underflow detected. \since 2.0.0
	KE_OVERFLOW,           //!< Overflow detected. \since 2.0.0
	KE_RANGE,              //!< Out of range. \since 2.0.0
}	kucode_t;

//! Set an error.
/*!
	Sets an error code and error text.
	\param ecode Error code.
	\param etext Error text.
	\return Error code (the same as \ref ecode parameter).
	\sa ku_rerror(), ku_gerrtx() and ku_gerrcode().
	\since 2.0.0
 */
kucode_t ku_serror( kucode_t ecode, const char *etext ) __THROW;

//! Reset the error state.
/*!
	Resets the error state. Error code is set to KE_NONE and
	error text is erased.
	\sa ku_serror(), ku_gerrtx() and ku_gerrcode().
	\since 2.0.0
*/
void ku_rerror( void ) __THROW;

//! Get a last error text.
/*!
	Gets text of the last error.
	\return Error text.
	\note Returned text should neither be freed nor realloced.
	\sa ku_serror(), ku_rerror() and ku_gerrcode().
	\since 2.0.0
 */
const char *ku_gerrtx( void ) __THROW;

//! Get a last error code.
/*!
	Gets error code of the last error.
	\return Error code (\ref kucode_t).
	\sa ku_serror(), ku_rerror() and ku_gerrtx().
	\since 2.0.0
 */
kucode_t ku_gerrcode( void ) __THROW;

//! Return the last error code from the function.
#define KU_ERRQ_PASS() \
preturnp("passed error") ku_gerrcode()

//! Set the error code and error text, and return the error code from the function.
#define KU_ERRQ( __ecode, __etext ) \
{ \
	kucode_t ecode = (__ecode); \
	const char *etext = (__etext); \
	preturnp("error: %d, '%s`", ecode, etext) ku_serror(ecode, etext); \
}

//! Set the error code and error text, and return the value from the function.
#define KU_ERRQ_V(__ecode, __etext, __value) \
{ \
	kucode_t ecode = (__ecode); \
	const char *etext = (__etext); \
	ku_serror(ecode, etext); \
	preturnp("error: %d, '%s`", ecode, etext) (__value); \
}

KU_END_DECLS
#endif
