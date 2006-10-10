#include <stdlib.h>

#include "ku2/debug.h"
#include "ku2/memory.h"
#include "ttp.h"

char *p_arg[PARSER_ARG_CNT];
int p_argc, p_size[PARSER_ARG_CNT];
char p_buf[BUFFER_SIZE_USER];

int parse( char *msg )
{
	int i=1, qu=0, tmp;
	char *src=msg;
	char *dst=p_buf;
	char *p;
	pstart();
	
	p_arg[0]=dst;
	if ( *src==0 ) return 0;
	while ( *src!=0 )
	{
		switch ( *src )
		{
			case ' ':
				if ( !qu )
				{
					*(dst++)=0;
					if ( i==PARSER_ARG_CNT )
					{
						plog(gettext("Cannot parse: too many parameters\n"));
						return 0;
					}	else
					{
						p_size[i-1]=dst-p_arg[i-1]-1;
						p_arg[i++]=dst;
					}
					src++;
				}	else
				{
					*(dst++)=*(src++);
				}
				break;
			case '/':
				tmp=strtol(++src,&p,0);
				if ( p==src )
				{
					plog(gettext("Cannot parse: after '/' must be a number\n"));
					return 0;
				}
				if ( (tmp<0) || (tmp>255) )
				{
					plog(gettext("Cannot parse: symbol code is invalid\n"));
					return 0;
				}
				*(dst++)=(char)tmp;
				src=p;
				break;
			case '@':
				if ( *(++src)!=0 )
					*(dst++)=*(src++);
				else
				{
					plog(gettext("Cannot parse: '@` at the end\n"));
					return 0;
				}
				break;
			case '"':
				qu=1-qu;
				src++;
				break;
			default:
				*(dst++)=*(src++);
		}
	}
	*dst=0;
	if ( qu )
	{
		plog(gettext("Cannot parse: quotation mark is needed\n"));
		return 0;
	}

	p_size[i-1]=dst-p_arg[i-1];
	p_argc=i;
	pstop();
	return i;
}
