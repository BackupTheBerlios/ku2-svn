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
#include <errno.h>

#include "io/cfgreader/cfg.h"
#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/memory.h"
#include "ds/abtree/abtree.h"
#include "other/other.h"
#include "dp/var/var.h"

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
	
	session->qtree = NULL;
	session->vsp = NULL;
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

#define CFG_QUERY_ERROR(__ecode) \
{ \
	va_end(va); \
	if ( qtree_cr == 1 ) \
	{ \
		abtree_free(session->qtree, qtree_free); \
		session->qtree = NULL; \
	} \
	KU_ERRQ(__ecode); \
}
kucode_t cfg_query( cfg_session_t *session, const char *rules, ... )
{
	va_list va;
	const char
		*cur = rules;	// текущий символ
	int cont,			// нужно ли обрабатывать дальше (continue?)
		qtree_cr;		// было ли создано дерево запросов?
	pstart();
	
	if ( session->qtree == NULL )
	{
		qtree_cr = 1;
		session->qtree = abtree_create(cfg_cmpf, 0);
		if ( session->qtree == NULL )
		{
			preturn KU_GET_ERROR();
		}
	}	else
		qtree_cr = 0;
	
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
			CFG_QUERY_ERROR(KE_SYNTAX);
		len_id = (cur++)-b_id;
		
		// поиск текущего формата
		b_fmt = cur;
		while ( (*cur != '/') && (*cur != ',') && *cur ) cur++;
		if ( *cur == 0 )
			cont = 0;
		len_fmt = cur-b_fmt;
		if ( len_fmt > 255 )
			CFG_QUERY_ERROR(KE_FULL);
		
		// поиск текущих опций
		if ( *cur == '/' )
		{
			unsigned long int l;
			char *p;
			
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
					CFG_QUERY_ERROR(KE_SYNTAX);
				default:
				{
					mode = 0;
					break;
				}
			}
			
			// кол-во обязательных параметров
			// после этих процедур указатель cur указывает на
			// следующий за значением символ
			errno = 0;
			l = strtoul(cur+2, &p, 10);
			if ( errno == ERANGE )
				CFG_QUERY_ERROR(KE_INVALID);
			if ( errno == EINVAL )
				comp = len_fmt; else
			{
				if ( l > (unsigned long int)len_fmt )
					CFG_QUERY_ERROR(KE_SYNTAX);
				comp = l;
			}
			if ( *p == 0 )
				cont = 0; else
			if ( *p != ',' )
				CFG_QUERY_ERROR(KE_SYNTAX);
			cur = p;
		}	else
		{
			mode = 0;
			comp = len_fmt;
		}

		// создание и добавление запроса в сессию
		q = (cfg_query_t*)dmalloc(sizeof(cfg_query_t) + \
								  sizeof(char)*(len_id+len_fmt+2) + \
								  sizeof(void**)*comp);
		if ( q == NULL )
			CFG_QUERY_ERROR(KE_MEMORY);
		
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
			goto __cfg_inherited_error;
		}
	}
	va_end(va);

	preturn KE_NONE;
	
__cfg_inherited_error:
	va_end(va);
	if ( qtree_cr == 1 )
	{
		abtree_free(session->qtree, qtree_free);
		session->qtree = NULL;
	}
	preturn KU_GET_ERROR();
}

// пропустить пробелы
static inline void cfg_sksp( char **p )
{
	while ( isspace(**p) && (**p != 0) ) (*p)++;
}

// прочитать следующее слово
// возвращает NULL, если нет закрывающей кавычки
// p переносится на начало последующего слова
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

/*
	Внимание, отсутствие символов межу разделителями
	распознаётся как пустая строка; если как тип параметра
	указан не STRING, то результат - синтактическая ошибка
*/
kucode_t cfg_process( cfg_session_t *session )
{
	char buf[CFG_BUFFER];
	cfg_query_t sq, *q;
	int quota = 0;
	pstart();
	
	while ( fgets(buf, CFG_BUFFER, session->cfgf) != NULL )
	{
		char *c = buf, *cur;
		uint i;
		uint len_fmt;
		
		session->cfg_line++;
		
		// пропуск комментариев
		cfg_sksp(&c);
		if ( *c == 0 ) continue;
		if ( *c == '#' )
		{
			if ( c[1] == '#' ) quota = 1-quota;
			continue;
		}
		if ( quota ) continue;
		
		// чтение идентификатора
		cur = cfg_readnext(&c);
		if ( cur == NULL )
			KU_ERRQ(KE_SYNTAX);
		
		// поиск идентификатора
		sq.id = cur;
		q = abtree_search(session->qtree, &sq);
		if ( q == NULL )
		{
			if ( session->flags&CFG_STRICT )
				KU_ERRQ(KE_NOTFOUND) else
			if ( !session->flags&CFG_DYNAMIC )
				continue;
		}
		
		// за идентификатором должно следовать равно
		if ( *c != '=' )
			KU_ERRQ(KE_SYNTAX);
		
		// чтение параметров
		if ( q != NULL )
			len_fmt = strlen(q->fmt); else
			len_fmt = 0; // динамическое распределение
		for ( i = 0; (len_fmt == 0) || (i < len_fmt); i++ )
		{
			char *p;
			
			// если параметров меньше, чем их обязательное кол-во,
			// то - ошибка
			if ( (*c == 0) && (q != NULL) && (i < q->comp) )
				KU_ERRQ(KE_SYNTAX);
			c++;
			
			// чтение одного параметра
			cur = cfg_readnext(&c);
			if ( cur == NULL )
				KU_ERRQ(KE_SYNTAX);
			
			// преобразование значения параметра
			if ( q != NULL )
			switch ( q->fmt[i] )
			{
				case VAL_INTEGER:
				{
					int res;
					if ( ku_strtoint(cur, &res) != KE_NONE )
						KU_ERRQ(KE_SYNTAX);
					if ( (q->mode == 0) || (q->mode == 'r') )
						*((int*)q->ptr[i]) = res;
					if ( (q->mode == 0) || (q->mode == 'd') )
					{
						// obrabotka dinami4eskogo rezima..
					}
					break;
				}
				case VAL_LONGINT:
					KU_ERRQ(KE_NOIMPLEM);
					break;
				case VAL_DOUBLE:
					*((double*)q->ptr[i]) = strtod(cur, &p);
					if ( *p!=0 )
						KU_ERRQ(KE_SYNTAX);
					break;
				case VAL_STRING:
					strcpy((char*)q->ptr[i], cur);
					break;
				case VAL_BOOLEAN:
					if ( (strcmp(cur, "yes") && strcmp(cur, "true")) == 0 )
						*((int*)q->ptr[i]) = 1; else
							if ( (strcmp(cur, "no") && strcmp(cur, "false")) == 0 )
								*((int*)q->ptr[i]) = 0; else
									KU_ERRQ(KE_SYNTAX);
					break;
				default:
					KU_ERRQ(KE_SYNTAX);
			} else
			{
				// правила для этого идентификатора нет, записываем в
				// пространство переменных
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
