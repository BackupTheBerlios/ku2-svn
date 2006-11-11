/***************************************************************************
 *            cfg.h
 *
 *  Fri Aug 25 21:30:11 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Config file reader.
	
	The realization of config file reader.
	\author J. Anton
	\date Sat Aug 26 21:04:19 2006
	\version 1.2.0
*/

#ifndef KU__CFG_H__
#define KU__CFG_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/ecode.h"
#include "ku2/types.h"

//! Line buffer, maximum line length, can be read.
#define CFG_BUFFER 2048

//! Flags for cfg_process().
enum CFG_FLAGS
{
	CFG_ZFL = 0,	//!< Zero flag.
	CFG_STRICT = 1,	//!< Config file cannot content labels, which are not queried.
	CFG_STEP = 2	//!< Stop when any query found.
};

//! Query.
typedef
struct STRUCT_CFG_QUERY
{
	char *id;		//!< Config label.
	char *fmt;		//!< Data format.
	void **ptr;		//!< Data.
}	cfg_query_t;

//! Current line in config file.
extern int cfg_line;

//! Label of the last step.
extern const char *cfg_stepid;

//! Open a config file.
/*!
	Opens a config file and creates a query tree.
	\param file Config file.
	\retval KE_NONE No error.
	\retval KE_DOUBLE Config file is already openned.
	\retval KE_IO Failed to open a file.
	\retval KE_* abtree_create() errors.
	\sa cfg_close().
*/
kucode_t cfg_open( const char *file );

//! Close a config file.
/*!
	Closed a config file.
	\retval KE_NONE No error.
	\retval KE_EMPTY No config file were openned by cfg_open().
	\sa cfg_open().
*/
kucode_t cfg_close( void );

//! Add a config query.
/*!
	Adds a config query to the tree.
	\param id Comma separeted list of config labels.
	\param fmt Comma separeted list of data format. \n
	\b 'i' stands for \e int, \n
	\b 'f' stands for \e double, \n
	\b 's' stands for \e char* (string), \n
	\b 'b' stands for \e boolean \e int (0/1).
	\param ... Pointers to the variables, where data will be written.
	\retval KE_NONE No error.
	\retval KE_MEMORY Memory allocation error.
	\retval KE_SYNTAX Config label list does not match data format list.
	\retval KE_* abtree_ins() errors.
	\sa cfg_process().
*/
kucode_t cfg_query( const char *id, const char *fmt, ... );

//! Read a config file and get data.
/*!
	Reads a config file and get data, according to the queries.
	\param flags Flags (\ref CFG_FLAGS).
	\retval KE_NONE No error (all labels are read).
	\retval KE_SIGNAL No error (one label is read) (see \ref CFG_STEP).
	\retval KE_SYNTAX Syntax error.
	\retval KE_NOTFOUND Label in the file does not match any query (see \ref CFG_STRICT).
	\warning Function does not allocate memory for the variables, it supposes
	that memory pointed to by a query is already allocated. Function does not
	check the extern buffer size for a string, but it is known that output
	string cannot be larger than \ref CFG_BUFFER bytes.
	\note \ref cfg_line is set to the last line read (error line).
	\note \ref cfg_stepid is set to the label of the last step (see \ref CFG_STEP).
	\sa cfg_query().
*/
kucode_t cfg_process( ku_flag32_t flags );

#ifdef __cplusplus
}
#endif
#endif
