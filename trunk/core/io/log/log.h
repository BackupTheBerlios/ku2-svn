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

#include <stdio.h>

#include "ku2/ecode.h"
#include "ku2/types.h"

//! Logging flags.
enum LOG_FLAGS
{
	LOG_ZFL = 0,	//!< Zero flag.
	LOG_DEFAULT = 1,
					//!< Make the created stream as default log destination.
	LOG_NDEFAULT = 2,
					//!< Override the default LOG_DEFAULT for ku_closelog().
	LOG_NHEAD = 4,	//!< No header and footer of the logging session.
	
	LOG_FLUSH = 8	//!< Flush the stream after every write.
};

//! The log.
typedef
struct STRUCT_LOG
{
	ku_flag32_t flags;
					//!< Default logging flags.
	FILE *logstream;
					//!< Log output stream.
}	ku_log;

//! Open a log file.
/*!
	Opens a file for appending a log messages. Starts the logging session.
	\param file A log filename.
	\retval KE_NONE No error.
	\retval KE_IO Cannot open a file.
	\sa ku_closelog(), ku_touchlog() and ku_plog().
*/
kucode_t ku_openlog( ku_log *thelog, const char *file, ku_flag32_t flags ) __THROW;

//! Set the default log.
/*!
	Sets the default log.
	\param thelog The log to become a default one.
	\sa ku_openlog(), ku_closelog(), ku_touchlog() and ku_plog().
*/
void ku_defaultlog( ku_log *thelog ) __THROW;

//! Close a log file.
/*!
	Closes the log file. Stops the logging session.
	\retval KE_NONE No error.
	\retval KE_IO Cannot close a file.
	\sa openlog(), flushlog() and plog().
*/
kucode_t ku_closelog( ku_log *thelog, ku_flag32_t flags ) __THROW;

//! Flush the log file.
/*!
	Writes all buffered data to the disk.
	\sa openlog(), closelog() and plog().
*/
kucode_t ku_touchlog( ku_log *thelog ) __THROW;

//! Print a log message.
/*!
	Prints to a file a formated log message. Before the message date and time
	are printed.
	\param fmt Formated message.
	\note No newline symbol at the end of the message is added.
	\sa openlog(), closelog(), plog_adv() and flushlog().
*/
void ku_plog( ku_log *thelog, uint16_t code, const char *function, const char *info, const char *fmt, ... ) __THROW;

//! Print a log message with the function name.
/*!
	Prints to a file a formated log message. Before the message date, time
	and the current function are printed.
	\param fmt Formated message.
*/
#define plog( fmt, ... ) \
ku_plog(NULL, 10000, NULL, NULL, fmt, ##__VA_ARGS__)

#define plogfn( fmt, ... ) \
ku_plog(NULL, 10000, __FUNCTION__, NULL, fmt, ##__VA_ARGS__)

#define plogfn_c( code, fmt, ... ) \
ku_plog(NULL, code, __FUNCTION__, NULL, fmt, ##__VA_ARGS__)

#define plogfn_i( info, fmt, ... ) \
ku_plog(NULL, 10000, __FUNCTION__, info, fmt, ##__VA_ARGS__)

#define plogfn_ci( code, info, fmt, ... ) \
ku_plog(NULL, code, __FUNCTION__, info, fmt, ##__VA_ARGS__)

//! Print a log message 'success!` with the function name.
#define KU_LOG_SUCCESS plogfn(gettext("success!\n"))

#ifdef __cplusplus
}
#endif
#endif
