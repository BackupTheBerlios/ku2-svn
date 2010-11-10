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
*/

#ifndef KU__OTHER_H__
#define KU__OTHER_H__
#include "ku2/host.h"
KU_BEGIN_DECLS

#include "ku2/ecode.h"
#include "ku2/types.h"

//! \name ESC sequences for setting the text output attributes.
//! \{
#define ESC_BEGIN( __txt ) "\033[0m"__txt
#define ESC_END( __txt ) __txt"\033[0m"
#define ESC_CBLACK( __txt ) "\033[30m"__txt
#define ESC_CRED( __txt ) "\033[31m"__txt
#define ESC_CGREEN( __txt ) "\033[32m"__txt
#define ESC_CYELLOW( __txt ) "\033[33m"__txt
#define ESC_CBLUE( __txt ) "\033[34m"__txt
#define ESC_CMAGENTA( __txt ) "\033[35m"__txt
#define ESC_CCYAN( __txt ) "\033[36m"__txt
#define ESC_CWHITE( __txt ) "\033[37m"__txt

#define ESC_BOLD( __txt ) "\033[0;1m"__txt"\033[0m"
#define ESC_UNDERLINED( __txt ) "\033[0;4m"__txt"\033[0m"
#define ESC_BLINK( __txt ) "\033[0;5m"__txt"\033[0m"

#define ESC_BLACK( __txt ) "\033[0;30m"__txt"\033[0m"
#define ESC_RED( __txt ) "\033[0;31m"__txt"\033[0m"
#define ESC_GREEN( __txt ) "\033[0;32m"__txt"\033[0m"
#define ESC_YELLOW( __txt ) "\033[0;33m"__txt"\033[0m"
#define ESC_BLUE( __txt ) "\033[0;34m"__txt"\033[0m"
#define ESC_MAGENTA( __txt ) "\033[0;35m"__txt"\033[0m"
#define ESC_CYAN( __txt ) "\033[0;36m"__txt"\033[0m"
#define ESC_WHITE( __txt ) "\033[0;37m"__txt"\033[0m"

#define ESC_BBLACK( __txt ) "\033[0;1;30m"__txt"\033[0m"
#define ESC_BRED( __txt ) "\033[0;1;31m"__txt"\033[0m"
#define ESC_BGREEN( __txt ) "\033[0;1;32m"__txt"\033[0m"
#define ESC_BYELLOW( __txt ) "\033[0;1;33m"__txt"\033[0m"
#define ESC_BBLUE( __txt ) "\033[0;1;34m"__txt"\033[0m"
#define ESC_BMAGENTA( __txt ) "\033[0;1;35m"__txt"\033[0m"
#define ESC_BCYAN( __txt ) "\033[0;1;36m"__txt"\033[0m"
#define ESC_BWHITE( __txt ) "\033[0;1;37m"__txt"\033[0m"
//! \}

//! Get the largest value.
#define MAXint( a, b ) ((a>b)?(a):(b))

//! Get the smallest value
#define MINint( a, b ) ((a<b)?(a):(b))

//! Convert the macros content to the constant string.
#define STR( m ) _STR_(m)

//! Convert the macros name to the constant string.
#define _STR_( m ) #m

//! Concatenate two constant strings.
#define CONCAT( m, n ) m ## n

//! String length for qstr() and vstr().
#define QSTR_STRSIZE 2048

//! String count in internal use of qstr() and vstr().
#define QSTR_STRCNT 16

//! Concatenate two strings.
/*!
	Concatenates two strings.
	\param s1 First string.
	\param s2 Second string.
	\return Pointer to the concatenated string.
	\note Concatenated string does not need to be freed.
	\note Concatenated string should be shorter then \ref QSTR_STRSIZE.
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
	\sa vstrts(), vstr_adv() and qstr().
*/
char *vstr( const char *fmt, ... );

//! Create a string from the format (thread-safe version).
/*!
	Creates a string using the specified format, like the printf() function.
	\param fmt Format.
	\return Pointer to the created string.
	\note Created string does not need to be freed.
	\note Created string should be shorter then \ref QSTR_STRSIZE.
	\note Function is not thread-safe.
	\warning Function does not perform any buffer overflow check. If \b DEBUG is
	defined then the warning will be sent to the \e stdout.
	\sa vstr(), vstr_adv() and qstr().
*/
char *vstrts( const char *fmt, ... );

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

//! Convert string to boolean/int.
/*!
	Converts string to boolean/int.
	\param str String to be converted.
	\param i Integer (1/0) to be saved to.
	\retval KE_NONE No errors.
	\retval KE_INVALID Invalid boolean.
	\sa ku_strtoint(), ku_strtouint() and ku_strtolong().
*/
kucode_t ku_strtobool( const char *str, int *i );

//! Get the power of 10.
/*!
	Gets the power of 10.
	\param pow The exponent of power of 10.
	\return The power of 10.
	\note \ref kucode is not affected.
*/
uint ku_pow10ui( uint pow );

KU_END_DECLS
#endif
