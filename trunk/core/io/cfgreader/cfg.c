/***************************************************************************
 *            cfg.c
 *
 *  Sat Aug 26 19:52:29 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "io/cfgreader/cfg.h"
#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/memory.h"
#include "ds/abtree/abtree.h"
#include "other/other.h"

static int cfg_cmpf( const void *data1, const void *data2 )
{
	return strcmp(((cfg_query_t*)data1)->id, ((cfg_query_t*)data2)->id);
}

static int qtree_free( void *data )
{
	dfree(data);
	return 0;
}

cfg_session_t *cfg_open( const char *file, ku_flag32_t flags )
{
	cfg_session_t *session;
	pstart();

	session = dmalloc(sizeof(cfg_session_t));
	if ( session == NULL )
	{
		KU_SET_ERROR(KE_MEMORY);
		preturn NULL;
	}

	session->cfgf = fopen(file, "r");
	if ( session->cfgf == NULL )
	{
		dfree(session);
		KU_SET_ERROR(KE_IO);
		preturn NULL;
	}

	session->qtree = abtree_create(cfg_cmpf, 0);
	if ( session->qtree == NULL )
	{
		fclose(session->cfgf);
		dfree(session);
		preturn NULL;
	}

	session->cfg_line = 0;
	session->flags = flags;

	preturn session;
}

kucode_t cfg_close( cfg_session_t *session )
{
	pstart();

	fclose(session->cfgf);
	abtree_free(session->qtree, qtree_free);
	dfree(session);

	preturn KE_NONE;
}

kucode_t cfg_query( cfg_session_t *session, const char *rules, ... )
{
	va_list va;
	const char
		*cur = rules;	// текущий символ
	int cont;			// нужно ли обрабатывать дальше (continue?)
	pstart();

	va_start(va, rules);
	for ( cont = 1; cont; cur++ )
	{
		cfg_query_t *q;
		uint i;
		const char
			*b_id,		// начало идентификатора
			*b_fmt;		// начала формата
		int	len_id,		// длина идентификатора
			len_fmt;	// длина формата
		uint8_t comp;	// кол-во обязательных параметров
		char mode;		// режим обработки
		
		// поиск текущего идентификатора
		b_id = cur;
		while ( (*cur != '=') && *cur ) cur++;
		if ( *cur == 0 )
		{
			va_end(va);
			KU_ERRQ(KE_SYNTAX);
		}
		len_id = (cur++)-b_id;
		
		// поиск текущего формата
		b_fmt = cur;
		while ( (*cur != '/') && (*cur != ',') && *cur ) cur++;
		if ( *cur == 0 )
			cont = 0;
		len_fmt = cur-b_fmt;
		if ( len_fmt > 255 )
		{
			va_end(va);
			KU_ERRQ(KE_FULL);
		}
		
		// поиск текущих опций
		if ( *cur == '/' )
		{
			unsigned long int l;
			
			// режим обработки
			switch ( cur[1] )
			{
				case 'd':
				case 'r':
				{
					mode = cur[1];
					break;
				}
				case 0:
				{
					va_end(va);
					KU_ERRQ(KE_SYNTAX);
				}
				default:
				{
					mode = 0;
					break;
				}
			}
			
			// кол-во обязательных параметров
			if ( ku_strtoulong(cur+2, &l) != KE_NONE )
			{
				va_end(va);
				preturn KU_GET_ERROR();
			}
			if ( l > (unsigned long int)len_fmt )
			{
				va_end(va);
				KU_ERRQ(KE_SYNTAX);
			}
			comp = l;
		}	else
		{
			mode = 0;
			comp = len_fmt;
		}
		
		// поиск конца правила
		for ( cur++; (*cur != ',' ) && *cur; cur++ );
		if ( *cur == 0 )
			cont = 0;

		// создание и добавление запроса в сессию
		q = (cfg_query_t*)dmalloc(sizeof(cfg_query_t) + \
								  sizeof(char)*(len_id+len_fmt+2) + \
								  sizeof(void**)*comp);
		if ( q == NULL )
		{
			va_end(va);
			KU_ERRQ(KE_MEMORY);
		}

		q->id = ((char*)q)+sizeof(cfg_query_t);
		q->fmt = ((char*)q->id)+sizeof(char)*(len_id+1);
		q->ptr = (void**)(((char*)q->fmt)+sizeof(char)*(len_fmt+1));

		strncpy(q->id, b_id, len_id);
		q->id[len_id] = 0;
		strncpy(q->fmt, b_fmt, len_fmt);
		q->fmt[len_fmt] = 0;
		q->comp = comp;
		q->mode = mode;

		for ( i = 0; i < (uint)(len_fmt); i++ )
			q->ptr[i] = va_arg(va, void*);

		if ( abtree_ins(session->qtree, q) != KE_NONE )
		{
			dfree(q);
			va_end(va);
			preturn KU_GET_ERROR();
		}
	}
	va_end(va);

	preturn KE_NONE;
}

// пропустить пробелы
static inline void cfg_sksp( char **p )
{
	while ( isspace(**p) && (**p != 0) ) (*p)++;
}

// прочитать следующее слово
static inline char *cfg_readnext( char **p )
{
	static char buf[CFG_BUFFER];
	char *c = buf;

	cfg_sksp(p);
	if ( **p == '"' )
	{
		(*p)++;
		while ( (**p != '"') && (**p != 0) ) *(c++) = *((*p)++);
		if ( *((*p)++) == 0 ) return NULL;
	}	else
	{
		while ( isalnum(**p) || (**p == '_') ) *(c++) = *((*p)++);
	}

	cfg_sksp(p);
	*c = 0;
	return buf;
}

kucode_t cfg_process( cfg_session_t *session )
{
	char buf[CFG_BUFFER];
	cfg_query_t sq, *q;
	int quota = 0;
	pstart();

	while ( fgets(buf, CFG_BUFFER, session->cfgf) != NULL )
	{
		char *c = buf, *cur;
		uint i, wassign;

		session->cfg_line++;

		cfg_sksp(&c);
		if ( *c == 0 ) continue;
		if ( *c == '#' )
		{
			if ( c[1] == '#' ) quota = 1-quota;
			continue;
		}

		if ( quota ) continue;

		cur = cfg_readnext(&c);
		if ( cur == NULL )
			KU_ERRQ(KE_SYNTAX);

		sq.id = cur;
		q = abtree_search(session->qtree, &sq);
		if ( q == NULL )
		{
			if ( session->flags&CFG_STRICT )
				KU_ERRQ(KE_NOTFOUND) else
				continue;
		}

		if ( *c != '=' )
			KU_ERRQ(KE_SYNTAX);
		wassign = 1;

		for ( i = 0; i < strlen(q->fmt); i++ )
		{
			char *p;

			if ( *c == 0 )
				KU_ERRQ(KE_SYNTAX);
			if ( (wassign || (*c == ',')) && !(wassign && (*c == ',')) )
			{
				wassign = 0;
				c++;
			}	else
				KU_ERRQ(KE_SYNTAX);
			cur = cfg_readnext(&c);
			if ( cur == NULL )
				KU_ERRQ(KE_SYNTAX);

			switch ( q->fmt[i] )
			{
				case 'i':
					*((int*)q->ptr[i]) = strtol(cur, &p, 0);
					if ( *p != 0 )
						KU_ERRQ(KE_SYNTAX);
					break;
				case 'f':
					*((double*)q->ptr[i]) = strtod(cur, &p);
					if ( *p!=0 )
						KU_ERRQ(KE_SYNTAX);
					break;
				case 's':
					strcpy((char*)q->ptr[i], cur);
					break;
				case 'b':
					if ( (strcmp(cur, "yes") && strcmp(cur, "true")) == 0 )
						*((int*)q->ptr[i]) = 1; else
							if ( (strcmp(cur, "no") && strcmp(cur, "false")) == 0 )
								*((int*)q->ptr[i]) = 0; else
									KU_ERRQ(KE_SYNTAX);
					break;
				default:
					KU_ERRQ(KE_SYNTAX);
			}
		}
		if ( *c != 0 )
			KU_ERRQ(KE_SYNTAX);

		if ( session->flags&CFG_STEP )
		{
			session->cfg_stepid = sq.id;
			preturn KE_SIGNAL;
		}
	}

	if ( quota )
		KU_ERRQ(KE_SYNTAX);

	preturn KE_NONE;
}
