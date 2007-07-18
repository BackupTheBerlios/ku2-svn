/*
		var.c
		Sat Jul 14 21:21:41 2007

	This file is the part of Kane Utilities 2.
	See licencing agreement in a root direcory for more details.
	http://developer.berlios.de/projects/ku2/

	Copyright, 2007
		J. Anton (Jeļkins Antons) aka Kane
		kane@mail.berlios.de
*/

#include <stdarg.h>
#include <string.h>

#include "ku2/ecode.h"
#include "dp/var/vlist.h"
#include "dp/var/var.h"
#include "ku2/memory.h"

// Буфер для кэша
#define VAR_ENABLE_BUFFERING

#ifdef VAR_ENABLE_BUFFERING
#define VAR_BUFSIZE		64

#define PARSE_SCALAR_VALUE( __ret, __type ) \
size += sizeof(__type); \
if ( var_bpos+sizeof(__type) > VAR_BUFSIZE ) \
{ \
	if ( buffed_pars == -1 ) \
	{ \
		buffed_pars = i; \
		va_copy(buffed_ap, ap); \
	} \
	__ret = va_arg(ap, __type); \
}	else \
{ \
	*((__type*)(var_buffer+var_bpos)) = va_arg(ap, __type); \
	((int*)var_buffer)[i] = var_bpos; \
	var_bpos += sizeof(__type); \
}
#else	// VAR_ENABLE_BUFFERING
#define PARSE_SCALAR_VALUE( __ret, __type ) \
__ret = va_arg(ap, __type); \
size += sizeof(__type);
#endif	// VAR_ENABLE_BUFFERING

var_t *var_define( const char *name, const char *val_types, ... )
{
	var_t *var;
	va_list ap;
	pstart();
	
	va_start(ap, val_types);
	var = var_define_v(name, val_types, ap);
	va_end(ap);
	
	preturn var;
}

var_t *var_define_v( const char *name, const char *val_types, va_list var_ap )
{
	var_t *var;
	int i, params = strlen(val_types),
		namelen = strlen(name);
	#ifdef VAR_ENABLE_BUFFERING
	char var_buffer[VAR_BUFSIZE];
	int var_bpos;
	int buffed_pars = -1;
	va_list buffed_ap;
	#endif
	uint size = sizeof(var_t)+namelen+params+params*sizeof(void*)+2;
	char *p;
	union
	{
		int i;
		long int I;
		double f;
		char *s;
	}	ret;
	va_list ap;
	pstart();
	
	// Рачёт размера переменной + буфферизация
	va_copy(ap, var_ap);
	for ( var_bpos = sizeof(void*)*params, i = 0; i < params; i++ )
	{
		switch ( val_types[i] )
		{
			case VAL_INTEGER:
			case VAL_BOOLEAN:
				PARSE_SCALAR_VALUE(ret.i, int);
				break;
			case VAL_LONGINT:
				PARSE_SCALAR_VALUE(ret.I, long int);
				break;
			case VAL_DOUBLE:
				PARSE_SCALAR_VALUE(ret.f, double);
				break;
			case VAL_STRING:
			{
				int len;
				#ifdef VAR_ENABLE_BUFFERING
				va_list ap_tmp;
				va_copy(ap_tmp, ap);
				ret.s = va_arg(ap_tmp, char*);
				va_end(ap_tmp);
				#else
				ret.s = va_arg(ap, char*);
				#endif
				
				len = strlen(ret.s)+1;
				
				#ifdef VAR_ENABLE_BUFFERING
				if ( var_bpos+len > VAR_BUFSIZE )
				{
					if ( buffed_pars == -1 )
					{
						buffed_pars = i;
						va_copy(buffed_ap, ap);
					}
				}	else
				{
					strcpy(var_buffer+var_bpos, ret.s);
					((int*)var_buffer)[i] = var_bpos;
					var_bpos += len;
				}
				#endif
				size += sizeof(char)*len;
				break;
			}
			default:
				KU_ERRQ_VALUE(KE_INVALID, NULL);
		}
	}
	#ifdef VAR_ENABLE_BUFFERING
	if ( buffed_pars == -1 )
		buffed_pars = params;
	#endif
	va_end(ap);
	
	// Выделение памяти под переменную и заполнение метаданными
	var = dmalloc(size);
	if ( var == NULL )
		KU_ERRQ_VALUE(KE_MEMORY, NULL);
	var->name = (char*)var+sizeof(var_t);
	strcpy(var->name, name);
	var->val_types = var->name+namelen+1;
	strcpy(var->val_types, val_types);
	var->values = (void**)(var->val_types+params+1);
	
	// Заполнение данными
	i = 0;
	#ifdef VAR_ENABLE_BUFFERING
	p = (char*)(var->values);
	for ( ; i < buffed_pars; i++ )
		((void**)var_buffer)[i] += (int)p;
	memmove(var->values, var_buffer, var_bpos);
	
	if ( buffed_pars < params )
	{
		va_copy(ap, buffed_ap);
		p = (char*)var->values+var_bpos;
	#else
		va_copy(ap, var_ap);
		p = (char*)(var->values+params);
	#endif
		
		for ( ; i < params; i++ )
		{
			var->values[i] = p;
			switch ( val_types[i] )
			{
				case VAL_INTEGER:
				case VAL_BOOLEAN:
				{
					*((int*)p) = va_arg(ap, int);
					p += sizeof(int);
					break;
				}
				case VAL_LONGINT:
				{
					*((long int*)p) = va_arg(ap, long int);
					p += sizeof(long int);
					break;
				}
				case VAL_DOUBLE:
				{
					*((double*)p) = va_arg(ap, double);
					p += sizeof(double);
					break;
				}
				case VAL_STRING:
				{
					ret.s = va_arg(ap, char*);
					strcpy(p, ret.s);
					p += strlen(ret.s)+1;
					break;
				}
			}
		}
		
		va_end(ap);
	#ifdef VAR_ENABLE_BUFFERING
		va_end(buffed_ap);
	}
	#endif
	
	preturn var;
}

var_t *var_define_l( const char *name, vlist_t *vlist )
{
	var_t *var;
	vlist_node_t *node;
	int i, params = vlist->vals->size,
		namelen = strlen(name);
	uint size = sizeof(var_t)+namelen+params+params*sizeof(void*)+2+vlist->datasz;
	char *p;
	pstart();
	
	if ( vlist->datasz == 0 )
		KU_ERRQ_VALUE(KE_EMPTY, NULL);
	
	// Выделение памяти под переменную и заполнение метаданными
	var = dmalloc(size);
	if ( var == NULL )
		KU_ERRQ_VALUE(KE_MEMORY, NULL);
	var->name = (char*)(var+1);
	strcpy(var->name, name);
	var->val_types = var->name+namelen+1;
	var->values = (void**)(var->val_types+params+1);
	
	// Заполнение данными
	p = (char*)(var->values+params);
	dl_list_first(vlist->vals);
	for ( i = 0; !dl_list_offside(vlist->vals); i++ )
	{
		node = dl_list_next(vlist->vals);
		var->val_types[i] = node->val_type;
		var->values[i] = p;
		switch ( node->val_type )
		{
			case VAL_INTEGER:
			case VAL_BOOLEAN:
			{
				*((int*)p) = *(int*)(node+1);
				p += sizeof(int);
				break;
			}
			case VAL_LONGINT:
			{
				*((long int*)p) = *(long int*)(node+1);
				p += sizeof(long int);
				break;
			}
			case VAL_DOUBLE:
			{
				*((double*)p) = *(double*)(node+1);
				p += sizeof(double);
				break;
			}
			case VAL_STRING:
			{
				strcpy(p, (char*)(node+1));
				p += strlen((char*)(node+1))+1;
				break;
			}
			default:
			{
				dfree(var);
				KU_ERRQ_VALUE(KE_INVALID, NULL);
			}
		}
	}
	
	preturn var;
}

kucode_t var_undef( var_t *var )
{
	pstart();
	dfree(var);
	preturn KE_NONE;
}
