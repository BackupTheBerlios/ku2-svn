
/*!
	\file
	\brief Communication channels.
	
	Communication channels for parallel data transfer throug it
	(mainly for sockets).
	\author J. Anton
	\date
	\version 0.1.0
*/

#ifndef KU__CHANNEL_H__
#define KU__CHANNEL_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ku2/ecode.h"
#include "ku2/types.h"

typedef
struct
{
	int o2;
}	channel_t;

typedef
enum
{
	O1
}	channel_opt_t;

//! Open a log file.
/*!
	Opens a file for appending a log messages. Starts the logging session.
	\param file A log filename.
	\retval KE_NONE No error.
	\retval KE_IO Cannot open a file.
	\sa closelog(), flushlog() and plog().
*/
channel_t *channel_assign( int fd, uint streams, ku_flag32_t flags );

kucode_t channel_release( channel_t *channel );
kucode_t channel_setopt( channel_t *channel, channel_opt_t opt, void *data );
kucode_t channel_getopt( channel_t *channel, channel_opt_t opt, void *data );

kucode_t channel_write( channel_t *channel, uint stream, const char *buffer, uint size );
int channel_read( channel_t *channel, uint stream, const char *buffer, uint size );
kucode_t channel_update( channel_t *channel );

#ifdef __cplusplus
}
#endif
#endif
