/***************************************************************************
 *            other.c
 *
 *  Sun Oct 23 10:02:17 2005
 *  Copyright  2005  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

#include "other.h"
#include "ku2/gettext.h"

#ifdef DEBUG
#include "log/log.h"
#endif

char *qstr( const char *s1, const char *s2 )
{
	static char _qstr_[QSTR_STRCNT][QSTR_STRSIZE];
	static int i = -1;
	if ( (++i) >= QSTR_STRCNT ) i = 0;
	sprintf(_qstr_[i], "%s%s", s1, s2);
	#ifdef DEBUG
	if ( strlen(_qstr_[i]) >= QSTR_STRSIZE )
	{
		plog(gettext("WARNING(qstr): buffer overflow detected!"));
	}
	#endif
	return _qstr_[i];
}

char *vstr( const char *fmt, ... )
{
	va_list ap;
	static char _vstr_[QSTR_STRCNT][QSTR_STRSIZE];
	static int i = -1;
	va_start(ap, fmt);
	if ( (++i) >= QSTR_STRCNT ) i = 0;
	vsprintf(_vstr_[i], fmt, ap);
	va_end(ap);
	#ifdef DEBUG
	if ( strlen(_vstr_[i]) >= QSTR_STRSIZE )
	{
		plog(gettext("WARNING(vstr): buffer overflow detected!"));
	}
	#endif
	return _vstr_[i];
}

void qdir( char *path )
{
	char *c = path;
	while ( *c ) c++;
	if ( c[-1] != '/' )
	{
		c[0] = '/';
		c[1] = 0;
	}
}
