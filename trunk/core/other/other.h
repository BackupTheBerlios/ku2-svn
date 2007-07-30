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
	\note Concatinated string should be shorter then \ref QSTR_STRSIZE.
	\note Function is not thread-safe.
	\warning Function does not perform any buffer overflow check. If \b DEBUG is
	defined then the warning will be sent to the \e stdout.
	\sa vstr() and vstr_adv().
*/
char *qstr( const char *s1, const char *s2 );

//! Create a string from the format.
/*!
	Creates a string using the specified format, like the printf() function.
	\param fmt Format.
	\return Pointer to the created string.
	\note Created string does not need to be freed.
	\note Created string should be shorter then \ref QSTR_STRSIZE.
	\note Function is not thread-safe.
	\warning Function does not perform any buffer overflow check. If \b DEBUG is
	defined then the warning will be sent to the \e stdout.
	\sa vstr_adv() and qstr().
*/
char *vstr( const char *fmt, ... );

//! Create a string from the format (advanced).
/*!
	Creates a string using the specified format, like the printf() function.
	\param dst Place to store the created string.
	\param fmt Format.
	\return Pointer to \e dst.
	\warning Function does not perform any buffer overflow check. If there is
	no enough space in \e dst, then buffer overflow is occured.
	\sa vstr() and qstr().
*/
char *vstr_adv( char *dst, const char *fmt, ... );

//! Convert a string to the path string.
/*!
	Adds \b / to the end of the string if there is no \b /.
	\param path String to be converted.
	\note Memory which is refered by \e path should have place for additional \b /.
	In other case, buffer overflow is occured.
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
	\note Function is not thread-safe.
	\sa ku_mtime().
*/
char *ku_ttime( void );

//! Convert string to integer.
/*!
	Converts string to integer.
	\param str String to be converted.
	\param i Integer to be saved to.
	\retval KE_NONE No errors.
	\retval KE_INVALID Invalid number.
	\sa ku_strtouint(), ku_strtolong() and ku_strtoulong().
*/
kucode_t ku_strtoint( const char *str, int *i );

//! Convert string to unsigned integer.
/*!
	Converts string to unsigned integer.
	\param str String to be converted.
	\param i Unisgned integer to be saved to.
	\retval KE_NONE No errors.
	\retval KE_INVALID Invalid number.
	\sa ku_strtoint(), ku_strtolong() and ku_strtoulong().
*/
kucode_t ku_strtouint( const char *str, unsigned int *i );

//! Convert string to long integer.
/*!
	Converts string to long integer.
	\param str String to be converted.
	\param i Long integer to be saved to.
	\retval KE_NONE No errors.
	\retval KE_INVALID Invalid number.
	\sa ku_strtoint(), ku_strtouint() and ku_strtoulong().
*/
kucode_t ku_strtolong( const char *str, long int *i );

//! Convert string to unsigned long integer.
/*!
	Converts string to unsigned long integer.
	\param str String to be converted.
	\param i Long integer to be saved to.
	\retval KE_NONE No errors.
	\retval KE_INVALID Invalid number.
	\sa ku_strtoint(), ku_strtouint() and ku_strtolong().
*/
kucode_t ku_strtoulong( const char *str, unsigned long int *i );

//! Convert string to double.
/*!
	Converts string to double.
	\param str String to be converted.
	\param i Double to be saved to.
	\retval KE_NONE No errors.
	\retval KE_INVALID Invalid number.
	\sa ku_strtoint(), ku_strtouint() and ku_strtolong().
*/
kucode_t ku_strtodouble( const char *str, double *i );

//! Get the power of 10.
/*!
	Gets the power of 10.
	\param pow The exponent of power of 10.
	\return The power of 10.
	\note \ref kucode is not affected.
*/
uint ku_pow10ui( uint pow );

#ifdef __cplusplus
}
#endif
#endif
