/***************************************************************************
 *            dialogue.h
 *
 *  Sun Feb 12 19:18:34 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Console dialogue.
	
	User <-> programme console dialogue realization.
	\author J. Anton
	\date Sat Sep  2 20:59:03 2006
	\version 1.2.0
*/

#ifndef KU__DIALOGUE_H__
#define KU__DIALOGUE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/ecode.h"
#include <stdio.h>

//! Input buffer size.
/*!
	This is en input buffer size.
	Input buffer is used to store user input data and answer data.
*/
#define DLGUE_STRSIZE 256

//! Question type.
typedef
enum
{
	DLGUE_BOOL = 1,	//!< Boolean (y/n).
	DLGUE_INT = 2,	//!< Signed integer.
	DLGUE_STRING = 3,
					//!< Character string.
	DLGUE_FLOAT = 4,
					//!< Floating point value.
	DLGUE_CANCEL = 8,
					//!< Allow cancel.
	DLGUE_DEFAULT = 24
					//!< Allow default value.
}	dlgue_t;

//! Redirect the input/output streams.
/*!
	Redirects the input and output streams.
	\param in Input stream.
	\param out Outout stream.
	\note By default, streams are not defined.
*/
void dlgue_stream( FILE *in, FILE *out );

//! Ask for an input.
/*!
	Asks for an input from the user.
	\param question Question text.
	\param answer Pointer, where the reply are be saved.
	\param type Input data type.
	\retval KE_NONE No error.
	\retval KE_IO Failed to read from the input stream.
	\retval KE_SYNTAX Input does not fit the buffer size (DLGUE_STRSIZE).
	\retval KE_EMPTY Default value or operation is canceled (empty string).
	\sa dlgue_claim().
	\note Possible \a answer types are \b int*, \b char**, \b double*.
	Answer will be pointed to the existing memory and no allocations
	and frees are needed.
*/
kucode_t dlgue_ask( const char *question, void *answer, dlgue_t type );

//! Output a text.
/*!
	Writes text to the output.
	\param text Formatted text to write out.
	\note Function does not add new line at the end.
	\sa dlgue_ask().
*/
void dlgue_claim( const char *text, ... );

#ifdef __cplusplus
}
#endif
#endif
