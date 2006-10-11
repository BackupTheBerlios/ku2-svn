/***************************************************************************
 *            other.h
 *
 *  Sun Oct 23 10:00:40 2005
 *  Copyright  2005  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Utilities.
	
	This file contains common used utilities.
	\author J. Anton
	\date Mon Aug 21 22:23:15 2006
	\version 1.1.0
*/

#ifndef KU__OTHER_H__
#define KU__OTHER_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/ecode.h"
#include "ku2/types.h"

//! Get the largest value.
#define MAXint( a, b ) ((a>b)?(a):(b))

//! Get the smallest value
#define MINint( a, b ) ((a<b)?(a):(b))

//! Convert the macros content to the constant string.
#define STR( m ) _STR_(m)

//! Convert the macros name to the constant string.
#define _STR_( m ) #m

//!	Concatinate two constant strings.
#define CONCAT( m, n ) m ## n

//! String length for qstr() and vstr().
#define QSTR_STRSIZE 2048

//! String count in internal use of qstr() and vstr().
#define QSTR_STRCNT 16

//! Concatinate two strings.
/*!
	Concatinates two strings.
	\param s1 First string.
	\param s2 Second string.
	\return Pointer to the concatinated string.
	\note Concatinated string does not need to be freed.
	\warning Function does not perform any buffer overflow check. If \b DEBUG is
	defined then the warning will be sent to the \e stdout.
	\sa vstr().
*/
char *qstr( const char *s1, const char *s2 );

//! Create a string from the format.
/*!
	Creates a string using the specified format, like the printf() function.
	\param fmt Format.
	\return Pointer to the created string.
	\note Created string does not need to be freed.
	\warning Function does not perform any buffer overflow check. If \b DEBUG is
	defined then the warning will be sent to the \e stdout.
	\sa qstr().
*/
char *vstr( const char *fmt, ... );

//! Convert a string to the path string.
/*!
	Adds \b / to the end of the string if there is no \b /.
	\param path String to be converted.
	\sa qdir2().
*/
void qdir( char *path );

//! Get the milliseconds.
/*!
	Gets the UNIX time in milliseconds.
	\return Milliseconds.
	\sa ku_ttime().
*/
uint ku_mtime( void );

//! Get the time string.
/*!
	Gets the time in form DD.MM.YYYY HH:MM:SS.
	\return Time string.
	\sa ku_mtime().
*/
char *ku_ttime( void );

//! Convert string to long integer.
/*!
	Converts string to long integer.
	\param str String to be converted.
	\param i Long integer to be saved to.
	\retval KE_NONE No errors.
	\retval KE_INVALID Invalid number.
	\sa ku_strtoulong().
*/
kucode_t ku_strtolong( const char *str, long int *i );

//! Convert string to unsigned long integer.
/*!
	Converts string to unsigned long integer.
	\param str String to be converted.
	\param i Long integer to be saved to.
	\retval KE_NONE No errors.
	\retval KE_INVALID Invalid number.
	\sa ku_strtolong().
*/
kucode_t ku_strtoulong( const char *str, unsigned long int *i );

#ifdef __cplusplus
}
#endif
#endif
