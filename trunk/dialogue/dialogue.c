/***************************************************************************
 *            dialogue.c
 *
 *  Sun Feb 12 19:18:40 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "dialogue.h"
#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/gettext.h"


FILE *fin = NULL, *fout = NULL;

void dlgue_stream( FILE *in, FILE *out )
{
	pstart();
	
	fin = in;
	fout = out;
	
	pstop();
}

kucode_t dlgue_ask( const char *question, void *answer, dlgue_t type )
{
	static struct
	{
		int _int;
		char _text[DLGUE_STRSIZE];
		double _real;
	}	ans;
	char *p;
	int isok = 0;
	pstart();
	
	do
	{
		fprintf(fout, question);
		if ( (type&DLGUE_DEFAULT) == DLGUE_DEFAULT )
		{
			fprintf(fout, gettext(" (ENTER means the default value)"));
			type |= DLGUE_CANCEL;
		}	else
		if ( (type&DLGUE_CANCEL) == DLGUE_CANCEL )
			fprintf(fout, gettext(" (ENTER cancels the operation)"));
		switch ( type&7 )
		{
			case DLGUE_BOOL:
			{
				fprintf(fout, " [y/n]: ");
				break;
			}
			case DLGUE_INT:
			{
				fprintf(fout, " [integer]: ");
				break;
			}
			case DLGUE_STRING:
			{
				fprintf(fout, " [text]: ");
				break;
			}
			case DLGUE_FLOAT:
			{
				fprintf(fout, " [real]: ");
				break;
			}
		}
		
		ans._text[DLGUE_STRSIZE-1] = 0;
		if ( fgets(ans._text, DLGUE_STRSIZE, fin) == NULL )
			KU_ERRQ(KE_IO);
		if ( ans._text[DLGUE_STRSIZE-1] != 0 )
			KU_ERRQ(KE_SYNTAX);
		
		if ( ((type&DLGUE_CANCEL) == DLGUE_CANCEL) && (ans._text[0] == '\n') )
		{
			KU_ERRQ(KE_EMPTY);
		}	else
		{
			switch ( type&7 )
			{
				case DLGUE_BOOL:
				{
					if ( !strcmp(ans._text, "y\n") )
						*((int*)answer) = 1; else
					if ( !strcmp(ans._text, "n\n") )
						*((int*)answer) = 0; else
							continue;
					break;
				}
				case DLGUE_INT:
				{
					ans._int = strtol(ans._text, &p, 0);
					if ( *p != 10 ) continue;
					*((int*)answer) = ans._int;
					break;
				}
				case DLGUE_STRING:
				{
					*((char**)answer) = ans._text;
					for ( p = ans._text; (*p != 10) && (*p != 0); p++ );
					*p = 0;
					break;
				}
				case DLGUE_FLOAT:
				{
					ans._real = strtod(ans._text, &p);
					if ( *p != 10 ) continue;
					*((double*)answer) = ans._real;
					break;
				}
			}
		}
		isok = 1;
	}	while ( !isok );
	
	pstop();
	return KE_NONE;
}

void dlgue_claim( const char *text, ... )
{
	va_list ap;
	pstart();
	
	va_start(ap, text);
	vfprintf(fout, text, ap);
	va_end(ap);
	
	pstop();
}