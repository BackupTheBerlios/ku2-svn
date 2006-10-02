#include <stdlib.h>

#include "ku2/debug.h"
#include "channel.h"

channel_t *channel_assign( int fd, uint streams, ku_flag32_t flags )
{
	pstart();
	pstop();
	return NULL;
}

kucode_t channel_release( channel_t *channel )
{
	pstart();
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
	pstop();
	return KE_NONE;
}

int channel_read( channel_t *channel, uint stream, const char *buffer, uint size )
{
	pstart();
	pstop();
	return 0;
}

kucode_t channel_update( channel_t *channel )
{
	pstart();
	pstop();
	return KE_NONE;
}
