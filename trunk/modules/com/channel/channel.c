#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "ku2/debug.h"
#include "ku2/memory.h"
#include "channel.h"

#ifdef VCPP
#pragma pack(1)
#endif
typedef
struct
{
	uint32_t length;
	uint8_t stream;
}	stream_head_t;

#ifdef WIN32
channel_t *channel_assign( SOCKET fd, uint streams, uint bufsz, ku_flag32_t flags )
#else
channel_t *channel_assign( int fd, uint streams, uint bufsz, ku_flag32_t flags )
#endif
{
	channel_t *chan;
	uint i;
	const uint offset = sizeof(channel_t)+4*sizeof(uint)*streams;
	pstart();

	if ( bufsz == 0 )
		bufsz = CHANNEL_BUFFER_SIZE;

	chan = dmalloc(sizeof(channel_t) + 2*streams*(bufsz+2*sizeof(uint)));
	if ( chan == NULL )
	{
		kucode = KE_MEMORY;
		return NULL;
	}

	chan->fd = fd;
	chan->bufsz = bufsz;
	chan->streams = streams;
	chan->max_block_size = STREAM_BLOCK_SIZE;

	chan->owpos = (uint*)((int8_t*)chan+sizeof(channel_t));
	chan->orpos = (uint*)((int8_t*)chan->owpos+sizeof(uint)*streams);
	chan->iwpos = (uint*)((int8_t*)chan->orpos+sizeof(uint)*streams);
	chan->irpos = (uint*)((int8_t*)chan->iwpos+sizeof(uint)*streams);

	chan->outbuf = (char*)((int8_t*)(chan)+offset);
	chan->inbuf = (char*)((int8_t*)(chan)+offset+bufsz*streams);

	for ( i = 0; i < streams; i++ )
		chan->owpos[i] = chan->orpos[i] = chan->iwpos[i] = chan->irpos[i] = 0;

	pstop();
	return chan;
}

kucode_t channel_release( channel_t *channel )
{
	pstart();

	dfree(channel);

	pstop();
	return KE_NONE;
}

kucode_t channel_setopt( channel_t *channel, channel_opt_t opt, void *data )
{
	pstart();

	switch ( opt )
	{
		case CHOPT_MAX_BLOCK_SZ:
		{
			channel->max_block_size = *(uint*)data;
			break;
		}
		default:
			KU_ERRQ(KE_INVALID);
	}

	pstop();
	return KE_NONE;
}

kucode_t channel_getopt( channel_t *channel, channel_opt_t opt, void *data )
{
	pstart();

	switch ( opt )
	{
		case CHOPT_MAX_BLOCK_SZ:
		{
			*(uint*)data = channel->max_block_size;
			break;
		}
		default:
			KU_ERRQ(KE_INVALID);
	}

	pstop();
	return KE_NONE;
}

kucode_t channel_write( channel_t *channel, uint stream, const char *buffer, uint size )
{
	pstart();

	ku_avoid(stream >= channel->streams);

	if ( size > channel->bufsz-channel->owpos[stream] )
		return KE_FULL;

	memcpy(channel->outbuf+stream*channel->bufsz+channel->owpos[stream], buffer, size);
	channel->owpos[stream] += size;

	pstop();
	return KE_NONE;
}

int channel_read( channel_t *channel, uint stream, char *buffer, uint size )
{
	pstart();

	ku_avoid_adv(stream >= channel->streams, -1);

	if ( size > channel->iwpos[stream]-channel->irpos[stream] )
	{
		kucode = KE_EMPTY;
		return -1;
	}

	memcpy(buffer, channel->inbuf+stream*channel->bufsz+channel->irpos[stream], size);
	channel->irpos[stream] += size;

	pstop();
	return 0;
}

/*
	THE CODE ABOVE IS CHECKED!
	AND DOES NOT NEED TO BE UPDATED FOR NOW,
	TODO: DATA NETWORK EXCHANGE. (LOWER)
*/
kucode_t channel_update( channel_t *channel )
{
	uint i, offset = 0;
	int bsr;	// bytes sent/received
	pstart();

	for ( ;; )
	{
		int no_action = 1;

		for ( i = 0; i < channel->streams; i++, offset += channel->bufsz )
		{
			int btsr ;	// bytes to send/receive

			btsr = channel->owpos[i]-channel->orpos[i];
			if ( btsr != 0 )
			{
				bsr = send(channel->fd, channel->outbuf+offset+channel->orpos[i], btsr, 0);
				#ifdef WIN32
				if ( bsr == SOCKET_ERROR )
				#else
				if ( bsr == -1 )
				#endif
					KU_ERRQ(KE_IO);



				no_action = 1;
			}
		}

		if ( no_action )
			break;
	}

	pstop();
	return KE_NONE;
}
