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

//! Get the largest value.
#define MAXint( a, b ) ((a>b)?(a):(b))

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

/*
	qdir + проверка существования директории
*/
int qdir2( char *path );

#ifdef __cplusplus
}
#endif
#endif
