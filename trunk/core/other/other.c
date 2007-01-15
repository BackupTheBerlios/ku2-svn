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
#include <limits.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <time.h>

#include "other.h"
#include "ku2/gettext.h"
#include "ku2/ecode.h"

#ifdef DEBUG
#include "io/log/log.h"
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

uint ku_mtime( void )
{
    struct timeb ttm;
    ftime(&ttm);
    return (uint)(ttm.time*1000+ttm.millitm);
}

char *ku_ttime( void )
{
	time_t t;
	struct tm *tm;
	time(&t);
	
	tm = localtime(&t);
	return vstr("%.2d.%.2d.%.4d %.2d:%.2d:%.2d", \
		tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900, \
		tm->tm_hour, tm->tm_min, tm->tm_sec);
}

kucode_t ku_strtolong( const char *str, long int *i )
{
	char *ep;
	long int res = strtol(str, &ep, 10);
	
	if ( (*str == 0) || (*ep != 0) || (res == LONG_MAX) || (res == LONG_MIN) )
	    KU_ERRQ(KE_INVALID);
	*i = res;

	return KE_NONE;
}

kucode_t ku_strtoulong( const char *str, unsigned long int *i )
{
	char *ep;
	long int res = strtoul(str, &ep, 10);

	if ( (*str == 0) || (*ep != 0) || (res == ULONG_MAX) )
	    KU_ERRQ(KE_INVALID);
	*i = res;

	return KE_NONE;
}
