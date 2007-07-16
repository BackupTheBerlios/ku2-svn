/***************************************************************************
 *            cfg.h
 *
 *  Fri Aug 25 21:30:11 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

/*!
	\file
	\brief Config and script file reader.

	The realization of config and script file reader.
	\author J. Anton
	\date Wed Jan 17 2007
	\version 1.3.0
*/

#ifndef KU__CFG_H__
#define KU__CFG_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "ku2/ecode.h"
#include "ku2/types.h"
#include "ds/abtree/abtree.h"
#include "dp/var/vspace.h"

//! Line buffer, maximum line length can be read.
#define CFG_BUFFER 2048

//! Session flags.
enum CFG_FLAGS
{
	CFG_ZFL = 0,	//!< Zero flag.

	/*!
		\brief Config file cannot content entries, which are not queried.

		Otherwise, all not queried entries will be ignored.
		\note This flag cannot be used with \ref CFG_DYNAMIC flag.
	*/
	CFG_STRICT = 1,
	CFG_STEP = 2,	//!< Stop when any entry found.

	/*!
		\brief Parse entries, which are not queried.

		All not queried entries will be recognised as list of text (s cfg_quert()) data format.
		\note Flag should be used with \ref CFG_STEP and cannot be used with \ref CFG_STRICT.
	*/
	CFG_DYNAMIC = 4
};

//! Session.
typedef
struct STRUCT_CFG_SESSION
{
	ku_flag32_t flags;
					//!< Session flags;
	FILE *cfgf;		//!< File handle.
	tree_t *qtree;	//!< Query tree (predefined variable rules).
	int cfg_line;	//!< Current file line.
	const char *cfg_stepid;
					//!< Last step id (see \ref CFG_STEP).
	vspace_t *vsp;	//!< Variable space (see \ref CFG_DYNAMIC).
}	cfg_session_t;

//! Query/predefined rule.
typedef
struct STRUCT_CFG_QUERY
{
	char *id;		//!< Data label.
	char *fmt;		//!< Data format.
	uint8_t comp;	//!< Number of compulsory parameters.
	char mode;		//!< Predefined query mode.
	void **ptr;		//!< Data.
}	cfg_query_t;

//! Open a configuration or script file session.
/*!
	Opens a configuration or script file session and creates a query tree.
	\param file Configuration or script file name.
	\param flags Session flags (see \ref CFG_FLAGS).
	\return Pointer to the opened session, else \e NULL and \ref kucode is
	set to the valid value: \n \a KE_MEMORY: Memory allocation error. \n
	\a KE_IO: Failed to open a file. \n \a KE_*: abtree_create() errors.
	\sa cfg_close().
*/
cfg_session_t *cfg_open( const char *file, ku_flag32_t flags );

//! Close a configuration or script file session.
/*!
	Closes a configuration or script file session.
	\param session Session to be closed.
	\return Always \a KE_NONE.
	\sa cfg_open().
*/
kucode_t cfg_close( cfg_session_t *session );

//! Add a config query.
/*!
	Adds a config query to the tree.
	\param session Session to add query to.
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
kucode_t cfg_query( cfg_session_t *session, const char *rules, ... );

//! Read a config file and get data.
/*!
	Reads a config file and get data, according to the queries.
	\param session Session to be processed.
	\retval KE_NONE No error (all labels are read).
	\retval KE_SIGNAL No error (one label is read) (see \ref CFG_STEP).
	\retval KE_SYNTAX Syntax error.
	\retval KE_NOTFOUND Label in the file does not match any query (see \ref CFG_STRICT).
	\warning Function does not allocate memory for the variables, it supposes
	that memory pointed to by a query is already allocated. Function does not
	check the extern buffer size for a string, but it is known that output
	string cannot be larger than \ref CFG_BUFFER bytes.
	\note session \e cfg_line is set to the last line read (error line).
	\note session \e cfg_stepid is set to the label of the last step (see \ref CFG_STEP).
	\sa cfg_query().
*/
kucode_t cfg_process( cfg_session_t *session );

#ifdef __cplusplus
}
#endif
#endif
