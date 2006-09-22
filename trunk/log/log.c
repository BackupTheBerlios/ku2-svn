/***************************************************************************
 *            log.c
 *
 *  Sat Oct 22 12:19:20 2005
 *  Copyright  2005  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>

#include "log.h"
#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/gettext.h"

FILE *logstream;

kucode_t openlog( const char *file )
{
	return openlog_adv(file, LOG_ZFL);
}

kucode_t openlog_adv( const char *file, ku_flag32_t flags )
{
	FILE *f;
	pstart();
	
	f = fopen(file, "a");
	if ( f == NULL )
		KU_ERRQ(KE_IO);
	
	logstream = f;

	if ( !(flags&LOG_NHEAD) )
	{
		fprintf(logstream, "========\n");
		plog(gettext("Logging has been started\n"));
	}
	
	pstop();
	return KE_NONE;
}

kucode_t closelog( void )
{
	return closelog_adv(LOG_ZFL);
}

kucode_t closelog_adv( ku_flag32_t flags )
{
	pstart();
	
	if ( !(flags&LOG_NHEAD) )
	{
		plog(gettext("Logging is being stopped\n"));
		fprintf(logstream, "========\n\n");
	}
	
	if ( fclose(logstream) != 0 )
	{
		logstream = NULL;
		KU_ERRQ(KE_IO);
	}
	
	logstream = NULL;
	
	pstop();
	return KE_NONE;
}

void flushlog( void )
{
	fflush(logstream);
}

void plog( const char *fmt, ... )
{
	va_list ap;
	time_t t;
	struct tm *tm;
	time(&t);
	
	tm = localtime(&t);
	
	va_start(ap, fmt);
	fprintf(logstream, "[%.2d.%.2d.%.4d %.2d:%.2d:%.2d] ", \
		tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900, \
		tm->tm_hour, tm->tm_min, tm->tm_sec);
	vfprintf(logstream, fmt, ap);
	#ifdef DEBUG_LOG
	printf("=== LOG === [%.2d.%.2d.%.4d %.2d:%.2d:%.2d] ", \
		tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900, \
		tm->tm_hour, tm->tm_min, tm->tm_sec);
	vprintf(fmt, ap);
	#endif
	va_end(ap);
}
