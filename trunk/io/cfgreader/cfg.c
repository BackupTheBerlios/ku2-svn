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

#include "cfg.h"
#include "ku2/ecode.h"
#include "ku2/debug.h"
#include "ku2/memory.h"
#include "ds/abtree/abtree.h"

FILE *cfgf = NULL;
tree_t *qtree;
int cfg_line;
const char *cfg_stepid;

static int cfg_cmpf( void *data1, void *data2 )
{
	return strcmp(((cfg_query_t*)data1)->id, ((cfg_query_t*)data2)->id);
}

static int qtree_free( void *data )
{
	dfree(data);
	return 0;
}

kucode_t cfg_open( const char *file )
{
	pstart();
	
	if ( cfgf != NULL )
		KU_ERRQ(KE_DOUBLE);
	
	cfgf = fopen(file, "r");
	if ( cfgf == NULL )
		KU_ERRQ(KE_IO);
	
	qtree = abtree_create(cfg_cmpf, 0);
	if ( qtree == NULL )
	{
		fclose(cfgf);
		return kucode;
	}
	
	cfg_line = 0;
	
	pstop();
	return KE_NONE;
}

kucode_t cfg_close( void )
{
	pstart();
	
	if ( cfgf == NULL )
		KU_ERRQ(KE_EMPTY);
	
	fclose(cfgf);
	cfgf = NULL;
	
	abtree_free(qtree, qtree_free);
	
	pstop();
	return KE_NONE;
}

kucode_t cfg_query( const char *id, const char *fmt, ... )
{
	cfg_query_t *q;
	uint i;
	va_list va;
	const char *c_id = id, *b_id ,
		*c_fmt = fmt, *b_fmt;
	int cont = 1;
	pstart();
	
	va_start(va, fmt);
	for ( cont = 1; cont; c_id++, c_fmt++ )
	{
		b_id = c_id;
		while ( (*c_id != ',') && *c_id ) c_id++;
		if ( !*c_id )
			cont = 0;
		
		b_fmt = c_fmt;
		while ( (*c_fmt != ',') && *c_fmt ) c_fmt++;
		if ( !*c_fmt )
			cont = 0;
		
		q = dmalloc(sizeof(cfg_query_t)+ \
				sizeof(char)*(c_id-b_id+c_fmt-b_fmt+2)+ \
				sizeof(void**)*(c_fmt-b_fmt));
		if ( q == NULL )
			KU_ERRQ(KE_MEMORY);
		
		q->id = ((char*)q)+sizeof(cfg_query_t);
		q->fmt = ((char*)q->id)+sizeof(char)*(c_id-b_id+1);
		q->ptr = (void**)(((char*)q->fmt)+sizeof(char)*(c_fmt-b_fmt+1));
		
		strncpy(q->id, b_id, c_id-b_id);
		q->id[c_id-b_id] = 0;
		strncpy(q->fmt, b_fmt, c_fmt-b_fmt);
		q->fmt[c_fmt-b_fmt] = 0;
		
		
		for ( i = 0; i < c_fmt-b_fmt; i++ )
			q->ptr[i] = va_arg(va, void*);
		
		if ( abtree_ins(qtree, q) != KE_NONE )
		{
			dfree(q);
			return kucode;
		}	
	}
	va_end(va);
	
	if ( *(c_id-1) || *(c_fmt-1) )
		KU_ERRQ(KE_SYNTAX);
	
	pstop();
	return KE_NONE;
}

static void cfg_sksp( char **p )
{
	while ( isspace(**p) && (**p != 0) ) (*p)++;
}

static char *cfg_readnext( char **p )
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

kucode_t cfg_process( ku_flag32_t flags )
{
	char buf[CFG_BUFFER];
	cfg_query_t sq, *q;
	int quota = 0;
	pstart();
	
	while ( fgets(buf, CFG_BUFFER, cfgf) != NULL )
	{
		char *c = buf, *cur;
		uint i, wassign;
		
		cfg_line++;
		
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
		q = abtree_search(qtree, &sq);
		if ( q == NULL )
		{
			if ( flags&CFG_STRICT )
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
		
		if ( flags&CFG_STEP )
		{
			cfg_stepid = sq.id;
			pstop();
			return KE_SIGNAL;
		}
	}
	
	if ( quota )
		KU_ERRQ(KE_SYNTAX);
	
	pstop();
	return KE_NONE;
}
