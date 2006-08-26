/***************************************************************************
 *            log.h
 *
 *  Sat Oct 22 12:18:58 2005
 *  Copyright  2005  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Log messages.
	
	This file contains functions for logging messages.
	\author J. Anton
	\date Mon Aug 21 22:04:12 2006
	\version 1.1.0
*/

#ifndef KU__LOG_H__
#define KU__LOG_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/ecode.h"

//! Open a log file.
/*!
	Opens a file for appending a log messages. Starts the logging session.
	\param file A log filename.
	\retval KE_NONE No error.
	\retval KE_IO Cannot open a file.
	\sa closelog() and plog().
*/
kucode_t openlog( const char *file );

//! Close a log file.
/*!
	Closes the log file. Stops the logging session.
	\retval KE_NONE No error.
	\retval KE_IO Cannot close a file.
	\sa openlog() and plog().
*/
kucode_t closelog( void );

//! Print a log message.
/*!
	Prints to a file a formated log message. Before the message date and time
	are printed.
	\note No newline symbol at the end of the message is added.
	\sa openlog() and closelog().
*/
void plog( const char *fmt, ... );

#ifdef __cplusplus
}
#endif
#endif
