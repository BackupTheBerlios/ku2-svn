#include <stdlib.h>

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
	uint i, offset = sizeof(channel_t)+2*(sizeof(uint)*streams);
	pstart();

	if ( bufsz == 0 )
		bufsz = CHANNEL_BUFFER_SIZE;

	chan = dmalloc(sizeof(channel_t) + 2*streams*(bufsz+sizeof(uint)));
	if ( chan == NULL )
	{
		kucode = KE_MEMORY;
		return NULL;
	}

	chan->fd = fd;
	chan->bufsz = bufsz;
	chan->wpos = (uint*)((int8_t*)chan+sizeof(channel_t));
	chan->rpos = (uint*)((int8_t*)chan->wpos+sizeof(uint)*streams);
	chan->max_block_size = STREAM_BLOCK_SIZE;
	
	for ( i = 0; i < streams; i++, offset += 2*bufsz )
	{
		chan->outbuf[i] = (int8_t*)(chan)+offset;
		chan->inbuf[i] = (int8_t*)(chan)+offset+bufsz;
		chan->wpos[i] = chan->rpos[i] = 0;
	}

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
	pstop();
	return KE_NONE;
}

kucode_t channel_getopt( channel_t *channel, channel_opt_t opt, void *data )
{
	pstart();
	pstop();
	return KE_NONE;
}

kucode_t channel_write( channel_t *channel, uint stream, const char *buffer, uint size )
{
	pstart();

	if ( size > channel->bufsz-channel->wpos[stream] )
		return KE_FULL;

	memcpy(channel->outbuf[stream]+channel->wpos[stream], buffer, size);
	channel->wpos[stream] += size;

	pstop();
	return KE_NONE;
}

int channel_read( channel_t *channel, uint stream, char *buffer, uint size )
{
	pstart();

	if ( size > channel->wpos[stream]-channel->rpos[stream] )
		return KE_EMPTY;

	memcpy(buffer, channel->inbuf[stream]+channel->rpos[stream], size);
	channel->rpos[stream] += size;

	pstop();
	return 0;
}

kucode_t channel_update( channel_t *channel )
{
	pstart();



	pstop();
	return KE_NONE;
}
