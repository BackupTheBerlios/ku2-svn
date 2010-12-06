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

static ku_log *deflog = NULL;

kucode_t ku_openlog( ku_log *thelog, const char *file, ku_flag32_t flags )
{
	pstart();

	thelog->logstream = fopen(file, "a");
	if ( thelog->logstream == NULL )
		KU_ERRQNT(KE_IO);

	if ( flags & LOG_DEFAULT )
		deflog = thelog;

	if ( !(flags & LOG_NHEAD) ) {
		fprintf(thelog->logstream, "========\n");
		ku_plog(thelog, 10000, NULL, NULL,
		        gettext("Logging has been started\n"));
		fflush(thelog->logstream);
	}

	thelog->flags = flags;

	preturn KE_NONE;
}

void ku_defaultlog( ku_log *thelog )
{
	pstart();

	deflog = thelog;

	pstop();
}

kucode_t ku_closelog( ku_log *thelog, ku_flag32_t flags )
{
	pstart();

	ku_avoid( thelog == NULL );

	if ( !((flags & LOG_NHEAD) || (thelog->flags & LOG_NHEAD)) ) {
		ku_plog(thelog, 10000, NULL, NULL,
		        gettext("Logging is being stopped\n"));
		fprintf(thelog->logstream, "========\n\n");
		fflush(thelog->logstream);
	}

	if ( deflog == thelog )
		deflog = NULL;

	if ( fclose(thelog->logstream) != 0 )
		KU_ERRQNT(KE_IO);

	preturn KE_NONE;
}

kucode_t ku_touchlog( ku_log *thelog )
{
	if ( thelog == NULL ) {
		if ( deflog == NULL )
			fflush(stdout);
		else
			fflush(deflog->logstream);
	} else {
		fflush(thelog->logstream);
	}
	return KE_NONE;
}

void ku_plog( ku_log *thelog, uint16_t code, const char *function,
              const char *info, const char *fmt, ... )
{
	va_list ap;
	time_t t;
	struct tm *tm;
	ku_log stdoutlog;
	time(&t);

	if ( thelog == NULL ) {
		if ( deflog == NULL ) {
			thelog = &stdoutlog;
			thelog->logstream = stdout;
			thelog->flags = LOG_ZFL;
		} else
			thelog = deflog;
	}

	tm = localtime(&t);

	va_start(ap, fmt);

	fprintf(thelog->logstream, "[%.2d.%.2d.%.4d %.2d:%.2d:%.2d] ",
	        tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900, tm->tm_hour,
	        tm->tm_min, tm->tm_sec);
	if ( code <= 9999 )
		fprintf(thelog->logstream, "[%.4u] ", code);
	if ( info != NULL )
		fprintf(thelog->logstream, "[%s] ", info);
	if ( function != NULL )
		fprintf(thelog->logstream, "[%s] ", function);
	vfprintf(thelog->logstream, fmt, ap);

	if ( thelog->flags & LOG_FLUSH )
		fflush(thelog->logstream);

#ifdef DEBUG_LOG
	if ( thelog != &stdoutlog )
	{
		printf("=== LOG === [%.2d.%.2d.%.4d %.2d:%.2d:%.2d] ",
				tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900,
				tm->tm_hour, tm->tm_min, tm->tm_sec);
		if ( code <= 9999 )
		printf("[%.4u] ", code);
		if ( info != NULL )
		printf("[%s] ", info);
		if ( function != NULL )
		printf("[%s] ", function);
		vprintf(fmt, ap);
	}
#endif

	va_end(ap);
}
