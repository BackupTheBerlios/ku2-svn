/*!
	\file
	\brief Communication channels.
	
	Communication channels for parallel data transfer throug it
	(for sockets).
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

#ifdef WIN32
#include <winsock2.h>
#endif

#define CHANNEL_BUFFER_SIZE 1024
#define STREAM_BLOCK_SIZE 4096

typedef
enum
{
	O1
}	channel_opt_t;

typedef
struct
{
	#ifdef WIN32
	SOCKET fd;
	#else
	int fd;
	#endif

	uint bufsz, streams;

	char *inbuf;
	uint *irpos, *iwpos;

	char *outbuf;
	uint *orpos, *owpos;

	//	options
	uint max_block_size;
}	channel_t;

//! Open a log file.
/*!
	Opens a file for appending a log messages. Starts the logging session.
	\param file A log filename.
	\retval KE_NONE No error.
	\retval KE_IO Cannot open a file.
	\sa closelog(), flushlog() and plog().
*/
#ifdef WIN32
channel_t *channel_assign( SOCKET fd, uint streams, uint bufsz, ku_flag32_t flags );
#else
channel_t *channel_assign( int fd, uint streams, uint bufsz, ku_flag32_t flags );
#endif

kucode_t channel_release( channel_t *channel );
kucode_t channel_setopt( channel_t *channel, channel_opt_t opt, void *data );
kucode_t channel_getopt( channel_t *channel, channel_opt_t opt, void *data );

kucode_t channel_write( channel_t *channel, uint stream, const char *buffer, uint size );
int channel_read( channel_t *channel, uint stream, char *buffer, uint size );
kucode_t channel_update( channel_t *channel );

#ifdef __cplusplus
}
#endif
#endif
