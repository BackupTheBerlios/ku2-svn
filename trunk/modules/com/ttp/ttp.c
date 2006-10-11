#include <stdlib.h>

#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ttp.h"

static char p_buf[TTP_BUFFER_SIZE];

int ttp_decode( const char *msg, char **args, int argc_max )
{
	int i = 1, qu = 0, tmp;
	int argc;
	const char *src = msg;
	char *dst = p_buf;
	char *p;
	pstart();
	
	args[0] = dst;
	if ( *src == 0 )
	{
		kucode = KE_EMPTY;
		return 0;
	}
	while ( *src != 0 )
	{
		switch ( *src )
		{
			case ' ':
			{
				if ( !qu )
				{
					*(dst++) = 0;
					if ( i == argc_max )
					{
						kucode = KE_FULL;
						return 0;
					}	else
					{
						args[i++] = dst;
					}
					src++;
				}	else
				{
					*(dst++) = *(src++);
				}
				break;
			}
			case '/':
			{
				tmp = strtol(++src, &p, 0);
				if ( (p == src) || (tmp<0) || (tmp>255) )
				{
					kucode = KE_INVALID;
					return 0;
				}
				*(dst++) = (char)tmp;
				src = p;
				break;
			}
			case '@':
			{
				if ( *(++src) != 0 )
					*(dst++) = *(src++);
				else
				{
					kucode = KE_SYNTAX;
					return 0;
				}
				break;
			}
			case '"':
			{
				qu = 1-qu;
				src++;
				break;
			}
			default:
				*(dst++) = *(src++);
		}
	}
	*dst = 0;
	if ( qu )
	{
		kucode = KE_SYNTAX;
		return 0;
	}

	argc = i;
	pstop();
	return i;
}
