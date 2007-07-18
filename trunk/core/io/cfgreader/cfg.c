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
#include "dp/var/vlist.h"
#include "dp/var/var.h"
#include "dp/var/vspace.h"

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
	session->cfg_line = 0;
	session->flags = flags;
	session->vsp = NULL;
	
	preturn session;
}

kucode_t cfg_close( cfg_session_t *session )
{
	pstart();

	fclose(session->cfgf);
	if ( session->qtree )
		abtree_free(session->qtree, qtree_free);
	if ( session->vsp )
		vspace_undef(session->vsp);
	dfree(session);

	preturn KE_NONE;
}

#define CFG_QUERY_ERROR(__ecode) \
{ \
	va_end(va); \
	KU_ERRQ(__ecode); \
}
kucode_t cfg_query( cfg_session_t *session, const char *rules, ... )
{
	va_list va;
	const char
		*cur = rules;	// текущий символ
	int cont;			// нужно ли обрабатывать дальше (continue?)
	pstart();
	
	if ( session->qtree == NULL )
	{
		session->qtree = abtree_create(cfg_cmpf, 0);
		if ( session->qtree == NULL )
		{
			preturn KU_GET_ERROR();
		}
	}
	
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

static inline int cfg_parse_directive( cfg_session_t *session, char *c, vspace_t **vspace_ptr )
{
	char *cur = cfg_readnext(&c);
	
	if ( !strcmp(cur, "end") )
	{
		
	}	else
	if ( !strcmp(cur, "section") )
	{
		
	}	else
		return 0;
	
	return 1;
}

static inline int cfg_parse_parameters( int has_rule, char *c, cfg_query_t *q, vlist_t **vlist_ptr )
{
	vlist_t *vlist = *vlist_ptr;
	uint i, len_fmt;
	char *cur;
	
	if ( has_rule )
		len_fmt = strlen(q->fmt);
	for ( i = 0;; i++ )
	{
		// если параметров меньше, чем их обязательное кол-во,
		// то - ошибка
		if ( *c == 0 )
		{
			if ( has_rule && (i < q->comp) )
				KU_ERRQ_VALUE(KE_SYNTAX, 0) else break;
		}
		// или если их больше, чем надо..
		if ( (i >= len_fmt) && has_rule )
			KU_ERRQ_VALUE(KE_SYNTAX, 0);
		c++;
		
		// чтение одного параметра
		cur = cfg_readnext(&c);
		if ( cur == NULL )
			KU_ERRQ_VALUE(KE_SYNTAX, 0);
		
		// преобразование значения параметра
		if ( has_rule )
		{
			union
			{
				int i;
				long int I;
				double f;
				char *s;
			}	ret;
			switch ( q->fmt[i] )
			{
				case VAL_INTEGER:
				{
					if ( ku_strtoint(cur, &ret.i) != KE_NONE )
						KU_ERRQ_VALUE(KE_SYNTAX, 0);
					if ( (q->mode == 0) || (q->mode == 'r') )
						*((int*)q->ptr[i]) = ret.i;
					break;
				}
				case VAL_LONGINT:
				{
					if ( ku_strtolong(cur, &ret.I) != KE_NONE )
						KU_ERRQ_VALUE(KE_SYNTAX, 0);
					if ( (q->mode == 0) || (q->mode == 'r') )
						*((long int*)q->ptr[i]) = ret.I;
					break;
				}
				case VAL_DOUBLE:
				{
					if ( ku_strtodouble(cur, &ret.f) != KE_NONE )
						KU_ERRQ_VALUE(KE_SYNTAX, 0);
					if ( (q->mode == 0) || (q->mode == 'r') )
						*((double*)q->ptr[i]) = ret.f;
					break;
				}
				case VAL_STRING:
				{
					ret.s = cur;
					if ( (q->mode == 0) || (q->mode == 'r') )
						strcpy((char*)q->ptr[i], ret.s);
					break;
				}
				case VAL_BOOLEAN:
				{
					if ( (strcmp(cur, "yes") && strcmp(cur, "true")) == 0 )
						ret.i = 1; else
							if ( (strcmp(cur, "no") && strcmp(cur, "false")) == 0 )
								ret.i = 0; else
									KU_ERRQ_VALUE(KE_SYNTAX, 0);
					if ( (q->mode == 0) || (q->mode == 'r') )
						*((int*)q->ptr[i]) = ret.i;
					break;
				}
				default:
					KU_ERRQ_VALUE(KE_INVALID, 0);
			}
			// добавление значений в динамический список
			if ( (q->mode == 0) || (q->mode == 'd') )
			{
				if ( vlist == NULL )
				{
					vlist = vlist_define();
					if ( vlist == NULL )
						{ preturn 0; }
					*vlist_ptr = vlist;
				}
				if ( q->fmt[i] == VAL_STRING )
				{
					if ( vlist_add(vlist, VAL_STRING, ret.s) != KE_NONE )
						{ preturn 0; }
				}	else
				{
					if ( vlist_add(vlist, q->fmt[i], &ret) != KE_NONE )
						{ preturn 0; }
				}
			}
		}	else
		{
			// правила для этого идентификатора нет, записываем в
			// пространство переменных
			if ( vlist == NULL )
			{
				vlist = vlist_define();
				if ( vlist == NULL )
					{ preturn 0; }
				*vlist_ptr = vlist;
			}
			if ( vlist_add(vlist, VAL_STRING, cur) != KE_NONE )
				{ preturn 0; }
		}
	}
	
	return 1;
}

/*
	Внимание, отсутствие символов межу разделителями
	распознаётся как пустая строка; если как тип параметра
	указан не STRING, то результат - синтактическая ошибка
*/
#define CFG_PROCESS_ERROR(__ecode) \
{ \
	__kucode = __ecode; \
	goto __cfg_error; \
}
kucode_t cfg_process( cfg_session_t *session )
{
	kucode_t __kucode;
	char buf[CFG_BUFFER];
	int quota = 0;
	vlist_t *vlist = NULL;
	vspace_t *vspace = session->vsp;
	pstart();
	
	// читаем, пока дают..
	errno = 0;
	while ( fgets(buf, CFG_BUFFER, session->cfgf) != NULL )
	{
		cfg_query_t sq, *q;
		char *c = buf, *cur, id_name[CFG_BUFFER];
		
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
		
		// обработка дериктив
		if ( *c == '&' )
		{
			if ( !cfg_parse_directive(session, c+1, &vspace) )
				goto __cfg_inherited_error; else
				continue;
		}
		
		// чтение идентификатора
		cur = cfg_readnext(&c);
		if ( cur == NULL )
			CFG_PROCESS_ERROR(KE_SYNTAX);
		
		// поиск идентификатора
		if ( session->qtree != NULL )
		{
			sq.id = cur;
			q = abtree_search(session->qtree, &sq);
		}	else
			q = NULL;
		if ( q == NULL )
		{
			if ( session->flags&CFG_STRICT )
				CFG_PROCESS_ERROR(KE_NOTFOUND) else
			if ( !session->flags&CFG_DYNAMIC )
				continue;
		}
		
		// за идентификатором должно следовать равно
		if ( *c != '=' )
			CFG_PROCESS_ERROR(KE_SYNTAX);
		
		// чтение параметров
		strcpy(id_name, cur);
		if ( !cfg_parse_parameters(q != NULL, c, q, &vlist) )
			goto __cfg_inherited_error;
		if ( (vlist != NULL) && (vlist->datasz > 0) )
		{
			if ( vspace == NULL )
			{
				session->vsp = vspace_define("");
				if ( session->vsp == NULL )
					goto __cfg_inherited_error;
				vspace = session->vsp;
			}
			if ( vspace_addv_l(vspace, id_name, vlist) != KE_NONE )
				goto __cfg_inherited_error;
			vlist_clear(vlist);
		}
		
		// если выбран пошаговый режим
		if ( session->flags&CFG_STEP )
		{
			if ( vlist )
				vlist_undef(vlist);
			session->cfg_stepid = id_name;
			preturn KE_SIGNAL;
		}
		
		errno = 0;
	}
	if ( errno != 0 ) // ошибка ввода/вывода
		CFG_PROCESS_ERROR(KE_IO);
	
	// освобождаем список значений
	if ( vlist != NULL )
		vlist_undef(vlist);
	
	// если не найдено окончание комментариев до конца файла, то ошибка
	if ( quota )
		KU_ERRQ(KE_SYNTAX);

	preturn KE_NONE;

__cfg_error:
	if ( vlist != NULL )
		vlist_undef(vlist);
	KU_ERRQ(__kucode);

__cfg_inherited_error:
	if ( vlist != NULL )
		vlist_undef(vlist);
	preturn KU_GET_ERROR();
}
