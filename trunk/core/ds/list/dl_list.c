/***************************************************************************
 *            dl_list.c
 *
 *  Wed Aug 23 21:54:02 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include "list.h"
#include "ku2/debug.h"
#include "ku2/memory.h"
#include "ku2/ecode.h"

list_t *dl_list_create( ku_comp_f func, ku_flag32_t flags __attribute__((unused)))
{
	list_t *ls;
	pstart();
	
	ls = (list_t*)dmalloc(sizeof(list_t));
	if ( ls == NULL )
	{
		kucode = KE_MEMORY;
		return NULL;
	}
	
	ls->size = 0;
	ls->ptr = NULL;
	ls->head = ls->tail = NULL;
	ls->cmpf = func;
	
	preturn ls;
}

kucode_t dl_list_free( list_t *list, ku_act_f freef )
{
	ls_node_t *node = list->head;
	ls_node_t *nnode;
	pstart();
	
	while ( node )
	{
		nnode = node->next;
		if ( freef )
			freef(node->data);
		dfree(node);
		node = nnode;
	}
	
	dfree(list);
	
	preturn KE_NONE;
}

kucode_t dl_list_ins( list_t *list, void *data )
{
	ls_node_t *node;
	pstart();

	node = (ls_node_t*)dmalloc(sizeof(ls_node_t));
	if ( node == NULL )
		KU_ERRQ(KE_MEMORY);

	node->data = data;
	
	if ( list->ptr )
	{
		node->prev = list->ptr->prev;
		node->next = list->ptr;
		if ( node->prev )
			node->prev->next = node; else
			list->head = node;
		node->next->prev = node;
	}	else
	{
		node->next = NULL;
		node->prev = NULL;
		list->head = list->tail = node;
	}

	list->ptr = node;
	list->size++;

	preturn KE_NONE;
}

kucode_t dl_list_ins_after( list_t *list, void *data )
{
	ls_node_t *node;
	pstart();
	
	node = (ls_node_t*)dmalloc(sizeof(ls_node_t));
	if ( node == NULL )
		KU_ERRQ(KE_MEMORY);
	
	node->data = data;
	
	if ( list->ptr )
	{
		node->prev = list->ptr;
		node->next = list->ptr->next;
		node->prev->next = node;
		if ( node->next )
			node->next->prev = node; else
			list->tail = node;
	}	else
	{
		node->next = NULL;
		node->prev = NULL;
		list->head = list->tail = node;
	}
	
	list->ptr = node;
	list->size++;
	
	preturn KE_NONE;
}

kucode_t dl_list_ins_last( list_t *list, void *data )
{
	ls_node_t *node;
	pstart();
	
	node = (ls_node_t*)dmalloc(sizeof(ls_node_t));
	if ( node == NULL )
		KU_ERRQ(KE_MEMORY);
	
	node->data = data;
	
	if ( list->tail )
	{
		node->prev = list->tail;
		node->next = NULL;
		node->prev->next = node;
	}	else
	{
		node->next = NULL;
		node->prev = NULL;
		list->head = list->ptr = node;
	}
	
	list->tail = node;
	list->size++;
	
	preturn KE_NONE;
}

kucode_t dl_list_rem( list_t *list, ku_act_f freef )
{
	ls_node_t *node = list->ptr;
	pstart();
	
	if ( node == NULL )
		KU_ERRQ(KE_EMPTY);
	
	if ( list->size == 1 )
	{
		list->ptr = list->head = list->tail = NULL;
	}	else
	{
		list->ptr = node->next;
		if ( node->prev )
			node->prev->next = node->next; else
			list->head = node->next;
		if ( node->next )
			node->next->prev = node->prev; else
			list->tail = node->prev;
	}
	
	if ( freef )
		freef(node->data);
	dfree(node);
	list->size--;
	
	preturn KE_NONE;
}

kucode_t dl_list_search( list_t *list, void *data )
{
	ls_node_t *cur = list->head;
	pstart();
	
	if ( cur == NULL )
		return KE_NOTFOUND;
	
	while ( cur && (list->cmpf(cur->data, data) != 0) )
		cur = cur->next;
	
	if ( cur == NULL )
		return KE_NOTFOUND;
	
	list->ptr = cur;
	
	preturn KE_NONE;
}

void *dl_list_cur( list_t *list )
{
	if ( list->ptr )
		return list->ptr->data; else
		return NULL;
}

void *dl_list_next( list_t *list )
{
	const ls_node_t *const node = list->ptr;
	
	if ( !node )
		return NULL;
	
	list->ptr = node->next;
	
	return node->data;
}

void *dl_list_prev( list_t *list )
{
	const ls_node_t *const node = list->ptr;
	
	if ( !node )
		return NULL;
	
	list->ptr = node->prev;
	
	return node->data;
}

int dl_list_islast( list_t *list )
{
	return !(list->ptr && list->ptr->next);
}

int dl_list_isfirst( list_t *list )
{
	return !(list->ptr && list->ptr->prev);
}

int dl_list_offside( list_t *list )
{
	return !list->ptr;
}

kucode_t dl_list_first( list_t *list )
{
	if ( !list->head )
		return KE_EMPTY;
	list->ptr = list->head;
	return KE_NONE;
}

kucode_t dl_list_last( list_t *list )
{
	if ( !list->tail )
		return KE_EMPTY;
	list->ptr = list->tail;
	return KE_NONE;
}

kucode_t dl_list_move_fwd( list_t *list )
{
	ls_node_t *const node = list->ptr;
	void *data;
	pstart();
	
	if ( !node )
		KU_ERRQ(KE_EMPTY);
	if ( !node->next )
		KU_ERRQ(KE_NOTFOUND);
	
	data = node->data;
	node->data = node->next->data;
	node->next->data = data;
	
	preturn KE_NONE;
}

kucode_t dl_list_move_back( list_t *list )
{
	ls_node_t *const node = list->ptr;
	void *data;
	pstart();
	
	if ( !node )
		KU_ERRQ(KE_EMPTY);
	if ( !node->prev )
		KU_ERRQ(KE_NOTFOUND);
	
	data = node->data;
	node->data = node->prev->data;
	node->prev->data = data;
	
	preturn KE_NONE;
}

kucode_t dl_list_move_first( list_t *list )
{
	ls_node_t *const node = list->ptr;
	pstart();
	
	if ( !node )
		KU_ERRQ(KE_EMPTY);
	
	if ( node->prev )
	{
		node->prev->next = node->next;
		if ( node->next )
			node->next->prev = node->prev; else
			list->tail = node->prev;
		node->prev = NULL;
		node->next = list->head;
		list->head = node;
	}
	
	preturn KE_NONE;
}

kucode_t dl_list_move_last( list_t *list )
{
	ls_node_t *const node = list->ptr;
	pstart();
	
	if ( !node )
		KU_ERRQ(KE_EMPTY);
	
	if ( node->next )
	{
		node->next->prev = node->prev;
		if ( node->prev )
			node->prev->next = node->next; else
			list->head = node->next;
		node->next = NULL;
		node->prev = list->tail;
		list->tail = node;
	}
	
	preturn KE_NONE;
}

kucode_t dl_list_delete_n( list_t *list, uint cnt, ku_act_f freef )
{
	ls_node_t *end_node = list->ptr, *nnode, *pre_node;
	pstart();
	
	if ( !end_node )
		KU_ERRQ(KE_EMPTY);
	
	pre_node = end_node->prev;
	
	while ( end_node && (cnt--) )
	{
		if ( freef )
			freef(end_node->data);
		nnode = end_node->next;
		dfree(end_node);
		end_node = nnode;
	}
	
	if ( pre_node )
		pre_node->next = end_node; else
		list->head = end_node;
	if ( end_node )
	{
		end_node->prev = pre_node;
		list->ptr = end_node;
	}	else
		list->ptr = list->tail = pre_node;
	
	preturn KE_NONE;
}
