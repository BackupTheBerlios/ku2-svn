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

ku_log *deflog;

kucode_t ku_openlog( ku_log *thelog, const char *file, ku_flag32_t flags )
{
	pstart();
	
	thelog->logstream = fopen(file, "a");
	if ( thelog->logstream == NULL )
		KU_ERRQ(KE_IO);
	
	if ( flags&LOG_DEFAULT )
		deflog = thelog;

	if ( !(flags&LOG_NHEAD) )
	{
		fprintf(thelog->logstream, "========\n");
		ku_plog(thelog, 0, NULL, NULL, gettext("Logging has been started\n"));
	}
	
	thelog->flags = flags;
	
	preturn KE_NONE;
}

kucode_t ku_closelog( ku_log *thelog, ku_flag32_t flags )
{
	pstart();
	
	if ( thelog == NULL )
		thelog = deflog;
	
	if ( !(flags&LOG_NHEAD) )
	{
		ku_plog(thelog, 0, NULL, NULL, gettext("Logging is being stopped\n"));
		fprintf(thelog->logstream, "========\n\n");
	}
	
	if ( thelog->flags&LOG_DEFAULT )
		deflog = NULL;
	
	if ( fclose(thelog->logstream) != 0 )
		KU_ERRQ(KE_IO);
	
	preturn KE_NONE;
}

kucode_t ku_toutchlog( ku_log *thelog )
{
	if ( thelog == NULL )
		thelog = deflog;
	fflush(thelog->logstream);
	return KE_NONE;
}

void ku_plog( ku_log *thelog, uint16_t code, const char *function, const char *info, const char *fmt, ... )
{
	va_list ap;
	time_t t;
	struct tm *tm;
	time(&t);
	
	if ( thelog == NULL )
		thelog = deflog;
	
	tm = localtime(&t);
	
	va_start(ap, fmt);
	
	fprintf(thelog->logstream, "[%.2d.%.2d.%.4d %.2d:%.2d:%.2d] ", \
		tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900, \
		tm->tm_hour, tm->tm_min, tm->tm_sec);
	if ( code != 0 )
		fprintf(thelog->logstream, "[%.4u] ", code);
	if ( function != NULL )
		fprintf(thelog->logstream, "[%s] ", function);
	if ( info != NULL )
		fprintf(thelog->logstream, "[%s] ", info);
	vfprintf(thelog->logstream, fmt, ap);
	
	#ifdef DEBUG_LOG
	printf("=== LOG === [%.2d.%.2d.%.4d %.2d:%.2d:%.2d] ", \
		tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900, \
		tm->tm_hour, tm->tm_min, tm->tm_sec);
	if ( code != 0 )
		printf("[%.4u] ", code);
	if ( function != NULL )
		printf("[%s] ", function);
	if ( info != NULL )
		printf("[%s] ", info);
	vprintf(fmt, ap);
	#endif
	
	va_end(ap);
}
