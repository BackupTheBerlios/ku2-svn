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
	\date Wed Aug 30 11:58:42 2006
	\version 1.2.0
*/

#ifndef KU__LOG_H__
#define KU__LOG_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/ecode.h"
#include "ku2/types.h"

//! Logging flags.
enum LOG_FLAGS
{
	LOG_ZFL = 0,	//!< Zero flag.
	LOG_NHEAD = 1	//!< No header and footer of the logging session.
};

//! Open a log file.
/*!
	Opens a file for appending a log messages. Starts the logging session.
	\param file A log filename.
	\retval KE_NONE No error.
	\retval KE_IO Cannot open a file.
	\sa closelog(), flushlog() and plog().
*/
kucode_t openlog( const char *file );

//! Open a log file (advanced).
/*!
	Opens a file for appending a log messages. Starts the logging session.
	\param file A log filename.
	\param flags Logging flags (options).
	\retval KE_NONE No error.
	\retval KE_IO Cannot open a file.
	\sa closelog(), flushlog() and plog().
*/
kucode_t openlog_adv( const char *file, ku_flag32_t flags );

//! Close a log file.
/*!
	Closes the log file. Stops the logging session.
	\retval KE_NONE No error.
	\retval KE_IO Cannot close a file.
	\sa openlog(), flushlog() and plog().
*/
kucode_t closelog( void );

//! Close a log file (advanced).
/*!
	Closes the log file. Stops the logging session.
	\param flags Logging flags (options).
	\retval KE_NONE No error.
	\retval KE_IO Cannot close a file.
	\sa openlog(), flushlog() and plog().
*/
kucode_t closelog_adv( ku_flag32_t flags );

//! Flush the log file.
/*!
	Writes all buffered data to the disk.
	\sa openlog(), closelog() and plog().
*/
void flushlog( void );

//! Print a log message.
/*!
	Prints to a file a formated log message. Before the message date and time
	are printed.
	\param fmt Formated message.
	\note No newline symbol at the end of the message is added.
	\sa openlog(), closelog(), plog_adv() and flushlog().
*/
void plog( const char *fmt, ... );

//! Print a log message (advanced).
/*!
	Prints to a file a formated log message. Before the message date, time
	and topic are printed.
	\param topic Message topic.
	\param fmt Formated message.
	\note No newline symbol at the end of the message is added.
	\sa openlog(), closelog(), plog() and flushlog().
*/
void plog_adv( const char *topic, const char *fmt, ... );

//! Print a log message with the function name.
/*!
	Prints to a file a formated log message. Before the message date, time
	and the current function are printed.
	\param fmt Formated message.
*/
#define plogfn( fmt, ... ) \
plog_adv(__FUNCTION__, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif
#endif
